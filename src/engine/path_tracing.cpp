#include "engine/path_tracing.h"

#include "geom/intersection.h"          // for Intersection
#include "geom/ray.h"                   // for Ray
#include "light/light.h"                // for Light
#include "material/material.h"          // for Material
#include "material/glass.h"
#include "math/frame.h"                 // for Frame
#include "node/group/group.h"           // for Group
#include "node/solid/solid.h"           // for Solid
#include "supersampling/supersampling.h"  // for SuperSampling
#include "world/world.h"                // for World

#include <assert.h>                     // for assert
#include <limits>                       // for numeric_limits
#include <memory>                       // for unique_ptr
#include <vector>                       // for vector

namespace Svit
{
	Vector3
  PathTracing::get_color (const Ray& _ray, const World& _world, 
                          SuperSampling* _sampler, const int _iteration, 
                          unsigned int& _ray_id) const
	{    
    Vector3 throughput(1.f,1.f,1.f);
    Vector3 accum(0.f,0.f,0.f);
    Intersection intersection(std::numeric_limits<float>::max());
    Vector3 brdfVal;
    float pdf;
    int pathLength=0;
    reflection_type refl_type=diffuse;
    bool inside_glass=false;
    Ray ray(_ray);
    while(true){
      ++pathLength;
      
      if( throughput.max() <=0 )
        break;
      
      bool scene_hit=_world.scene->intersect(ray,intersection);
      if(scene_hit && intersection.solid->light>=0 && (pathLength==1 || refl_type==reflection || refl_type==refraction)){
        const int lightID=intersection.solid->light;
        Vector3 illum=throughput * _world.lights[lightID]->get_radiance(ray.direction);
        if(illum.max()>0);
          accum += illum;
        break;
      }
      else if( (! scene_hit) || intersection.solid->light >= 0 ){ // no scene intersection
        break;
      }
      
      if(inside_glass){
        intersection.normal =! intersection.normal;
      }
      const Frame frame(intersection.normal);
      const int matID = intersection.solid->material ;
      if(inside_glass){
        Glass* glass = dynamic_cast<Glass*>(_world.materials[matID].get());
        if(glass==0)
          break;
      }
      const Vector3 wol=frame.to_local(! ray.direction);
      Vector2 samples_light=_sampler->next_sample();
      Vector2 samples_brdf=_sampler->next_sample();
      
      Vector3 illum=get_direct_illumination(frame,matID,intersection.point,wol,
                                       samples_brdf,samples_light,_world,inside_glass,_ray_id);
      if((throughput*illum).max()>0)
        accum+=(throughput*illum);
      
      // mame dalsi prusecik v ceste
      Vector3 wig;
      Vector2 samples=_sampler->next_sample();
      _world.materials[matID]->sample_brdf(intersection.point,frame,&pdf,wig,
                                           brdfVal,wol,samples,refl_type,
                                           inside_glass);
      
      float cosTheta=std::abs(wig % frame.normal);
      float reflectance=std::min(1.f,(brdfVal.max()*cosTheta)/pdf);
      if(_sampler->next_sample().x <= reflectance){
        throughput*=((brdfVal*cosTheta)/(reflectance*pdf));
        
        ray.origin=intersection.point;
        ray.direction=wig;
        ray.id=++_ray_id;
        intersection.t = std::numeric_limits<float>::max();
      }
      else{ // absorb
        break;
      }
    }
    return accum;    
	}
  
  Vector3
  PathTracing::get_direct_illumination(const Frame& frame, const int matID,
                          const Point3& surfpt, const Vector3& wol,
                          Vector2& samples_brdf, Vector2& samples_light,
                          const World& _world, bool inside_glass, 
                                       unsigned int& _ray_id) const
  {
    float lightDist=1;
    float pdfSampled=1.f;
    Vector3 wig;
    Vector3 brdfVal;
    Vector3 illum(0.f,0.f,0.f);
    Vector3 LoDirect(0.f,0.f,0.f);
    reflection_type refl_type;
    
    /*
    float weight;
    float pdfLight;
    float pdfBrdf;
    */
    
    
    // RANDOM LIGHT SAMPLING
    int light_count=_world.lights.size();
    unsigned int light_ID=(unsigned int) samples_light.x*light_count;
    if(light_ID == _world.lights.size())
      --light_ID;
    assert(light_ID < _world.lights.size());
    samples_light.x=(samples_light.x-(_world.lights_count_inv)*light_ID)*light_count;
    assert( samples_light.x>= 0.f && samples_light.x <=1.f );
    //sample light
    float pdfLight;
    illum=_world.lights[light_ID]->sample_light(surfpt,frame,samples_light,
                                                wig,lightDist,pdfLight);	
    brdfVal=_world.materials[matID]->eval_brdf(surfpt,frame.to_local(wig),wol);
    if(_world.lights[light_ID]->type==Point){
      if(! is_occluded(_world,surfpt,lightDist,wig,_ray_id)){
        assert(illum.max()>=0 && brdfVal.max()>=0);
        return illum * brdfVal;
      }
    }
    Ray ray_sec_light(surfpt,wig,_ray_id);
    Intersection isect_sec_light(std::numeric_limits<float>::max());
    
    //cast secondary ray
    if(_world.scene->intersect(ray_sec_light, isect_sec_light)){  
      if(isect_sec_light.solid->light >= 0){	//secondary ray hits light
        const int light_ID=isect_sec_light.solid->light;
        float pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                 frame.to_world(wol),wig);
        pdfLight=_world.lights[light_ID]->get_pdf(wig,lightDist)
                 *_world.lights_count_inv;
        
        //pdfBrdf=0.f; // //// ////////////////////////////////////////
        
        //assert();// && (pdfLight+pdfBrdf)>0);
        if(illum.max()>=0 && brdfVal.max()>=0)
          LoDirect += (illum * brdfVal  / (pdfLight + pdfBrdf));
      }
    }
    else // secondary ray doesnt intersect the scene
    {
      for(unsigned int i=0; i<_world.lights.size(); ++i){
        if(_world.lights[i]->type==Background){
          Vector3 illum = _world.lights[i]->get_radiance(wig)*(frame.normal % wig);
          pdfLight=_world.lights[i]->get_pdf(wig,lightDist)*_world.lights_count_inv;
          float pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                   frame.to_world(wol),wig);
          assert(brdfVal.max()>=0 && (pdfLight+pdfBrdf)>0);
          if(illum.max()>0)
            LoDirect += (illum * brdfVal / (pdfBrdf + pdfLight));    
        }
      }
    }
     
    // BRDF SAMPLING
    _world.materials[matID]->sample_brdf(surfpt, frame, &pdfSampled,wig,brdfVal,
                                         wol,samples_brdf, refl_type, inside_glass);
    Ray ray_sec_brdf(surfpt,wig,_ray_id);
    Intersection isect_sec_brdf(std::numeric_limits<float>::max());
    
    if(_world.scene->intersect(ray_sec_brdf, isect_sec_brdf))
    {  
      if(isect_sec_brdf.solid->light >= 0){	//secondary ray hits light
        const int light_ID=isect_sec_brdf.solid->light;
        illum = _world.lights[light_ID]->get_radiance(wig) * std::abs(frame.normal % wig);  
        lightDist=Vector3(ray_sec_brdf.direction*isect_sec_brdf.t) 
                  % Vector3(ray_sec_brdf.direction*isect_sec_brdf.t);
        float pdfLight=_world.lights[light_ID]->get_pdf(wig,lightDist)
                 *_world.lights_count_inv;
        
        // pdfLight=0; // //////////////////////////////////////////////
        
        float pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                 frame.to_world(wol),wig);
        float weight=computeMISWeight(pdfBrdf,pdfLight);
        assert( brdfVal.max()>=0 && weight>=0 && pdfSampled >0 );
        if( illum.max()>=0 )
          LoDirect += (illum * brdfVal * weight / pdfSampled);     
      }
    }
    else // secondary ray doesnt intersect the scene
    {
      for(unsigned int i=0; i<_world.lights.size(); ++i){
        if(_world.lights[i]->type==Background){
          Vector3 illum = _world.lights[i]->get_radiance(wig)*(frame.normal % wig);
          float pdfLight=_world.lights[i]->get_pdf(wig,lightDist)*_world.lights_count_inv;
          float pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                   frame.to_world(wol),wig);
          float weight=computeMISWeight(pdfBrdf,pdfLight);
          assert( brdfVal.max()>=0 && weight>=0 && pdfSampled >0);
          if(illum.max()>0)
            LoDirect += (illum * brdfVal  * weight / pdfSampled);
        }
      }
    }
    return LoDirect;
  }
}

