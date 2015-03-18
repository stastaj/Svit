#include "engine/path_tracing.h"

#include "geom/intersection.h"          // for Intersection
#include "geom/ray.h"                   // for Ray
#include "light/light.h"                // for Light
#include "material/material.h"          // for Material
#include "math/frame.h"                 // for Frame
#include "node/group/group.h"           // for Group
#include "node/solid/solid.h"           // for Solid
#include "supersampling/supersampling.h"  // for SuperSampling
#include "world/world.h"                // for World

#include <assert.h>                     // for assert
//#include <ext/alloc_traits.h>
#include <limits>                       // for numeric_limits
#include <memory>                       // for unique_ptr
#include <vector>                       // for vector

namespace Svit
{
	Vector3
  PathTracing::get_color (const Ray& _ray, const World& _world, 
                          SuperSampling* _sampler, const int _iteration) const
	{
    Intersection intersection(std::numeric_limits<float>::max());
    if(_world.scene->intersect(_ray,intersection)){      
      if(intersection.solid->light>=0){ // The light was hit by the ray
        return _world.lights[intersection.solid->light]->get_radiance(_ray.direction); 
      }
      
      
      Frame frame(intersection.normal); // tangent frame
      Vector3 wol= frame.to_local(! _ray.direction);
      Vector2 samples_light=_sampler->next_sample();
      Vector2 samples_brdf=_sampler->next_sample3();

      return get_direct_illumination(
            _iteration, frame, intersection.solid->material, intersection.point,
            wol, samples_brdf, samples_light, _world);
		}
		else
			return Vector3();
	}
  
  Vector3
  PathTracing::get_direct_illumination(int aIteration,const Frame& frame,const int matID,
                          const Point3& surfpt,const Vector3& wol,
                          Vector2& samples_brdf, Vector2& samples_light,
                          const World& _world) const
  {
    float lightDist=1;
    float pdfBrdf;				// brdf sampling
    float pdfLight;				// light sampling
    float weight;
    float pdfSampled=1.f;
    Vector3 wig;
    Vector3 brdfVal;
    Vector3 illum(0.f,0.f,0.f);
    Vector3 LoDirect(0.f,0.f,0.f);
    reflection_type refl_type;
    
    // generate secondary ray
    if(chooseSampling(aIteration)){ // brdf sampling
      _world.materials[matID]->sample_brdf(surfpt, frame, &pdfSampled,wig,
                                           brdfVal,wol,samples_brdf, refl_type);
    }
    else{ // random light sampling
      //get random light
      int light_count=_world.lights.size();
      int light_ID=(int) samples_light.x*light_count;
      samples_light.x=(samples_light.x-(1.f/light_count)*light_ID)*light_count;
      
      assert( samples_light.x>= 0.f && samples_light.x <=1.f );
      
      //sample light
      float pdfLight;
      illum=_world.lights[light_ID]->sample_light(surfpt,frame,samples_light,
                                                  wig,lightDist,pdfLight);	
      brdfVal=_world.materials[matID]->eval_brdf(surfpt,frame.to_local(wig),wol);
      if(_world.lights[light_ID]->type==Point){
        if(! is_occluded(_world,surfpt,lightDist,wig) && illum.max()>0)
          return illum * brdfVal ;
      }
    }
    
    Ray ray_secondary(surfpt,wig);
    Intersection intersection_sec(std::numeric_limits<float>::max());
    
    //cast secondary ray
    if(_world.scene->intersect(ray_secondary, intersection_sec))
    {  
      if(intersection_sec.solid->light >= 0){	//secondary ray hits light
        const int light_ID=intersection_sec.solid->light;
        if(chooseSampling(aIteration)){		// brdf sampling
          // vraci radianci * cosThetaX
          illum = _world.lights[light_ID]->get_radiance(wig) * (frame.mZ % wig);  
          lightDist=Vector3(ray_secondary.direction*intersection_sec.t) 
                    % Vector3(ray_secondary.direction*intersection_sec.t);
          pdfLight=_world.lights[light_ID]->get_pdf(wig,lightDist)
                   /_world.lights.size();
          
          // pdfLight=0; // //////////////////////////////////////////////
          
          pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                   frame.to_world(wol),wig);
          weight=computeMISWeight(pdfBrdf,pdfLight);
          if(illum.max()>0)
            LoDirect += (illum * brdfVal * 2.f * weight / pdfSampled);
        }
        else{		// area light sampling
          pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                   frame.to_world(wol),wig);
          pdfLight=_world.lights[light_ID]->get_pdf(wig,lightDist)
                   /_world.lights.size();
          
          //pdfBrdf=0.f; // //// ////////////////////////////////////////
          
          if(illum.max()>0)
            LoDirect += (illum * brdfVal * 2.f  / (pdfLight + pdfBrdf));
        }         
      }
    }
    else // secondary ray doesnt intersect the scene
    {
      for(int i=0; i<_world.lights.size(); i++){
        if(_world.lights[i]->type==Background){
          Vector3 illum = _world.lights[i]->get_radiance(wig)*(frame.mZ % wig);
          //lightDist=std::numeric_limits<float>::max();
          pdfLight=_world.lights[i]->get_pdf(wig,lightDist)/_world.lights.size();
          pdfBrdf=_world.materials[matID]->get_pdf(surfpt,frame,
                                                   frame.to_world(wol),wig);
          if(chooseSampling(aIteration)){
            weight=computeMISWeight(pdfBrdf,pdfLight);
            if(illum.max()>0)
              //LoDirect += (illum * brdfVal * weight / pdfSampled);
              LoDirect += (illum * brdfVal * 2.f * weight / pdfSampled);
          }
          else{
            if(illum.max()>0)
              //LoDirect += (illum * brdfVal / (pdfBrdf + pdfLight));
              LoDirect += (illum * brdfVal * 2.f / (pdfBrdf + pdfLight));
          }
              
        }
      }
    }
    return LoDirect;
  }
}

