#include "engine/path_tracing.h"

#include <numeric>
#include <cmath>

namespace Svit
{
	Vector3
  PathTracing::get_color (const Ray& _ray, const World& _world, 
                          SuperSampling* _sampler) const
	{
    Intersection intersection(std::numeric_limits<float>::max());
    if(_world.scene->intersect(_ray,intersection)){      
      if(intersection.solid->light>=0){ // The light was hit by the ray
        return _world.lights[intersection.solid->light]->get_radiance(_ray.direction); 
      }
      
      Vector3 illumination(0.f,0.f,0.f);
      for (auto &light : _world.lights)
      {
        Vector3 wig;
        float pdf;
        float dist;
        Vector2 samples=_sampler->next_sample();
        
        const Frame frame(intersection.normal);
        const Vector3 radiance=light->sample_light(intersection.point,frame,
                                                   samples,wig,dist,pdf);
        //assert();
        
        if (is_occluded(_world,intersection.point,dist,wig)){
          continue;
        }
        const Vector3 to_light=frame.to_local(wig);
        const Vector3 to_camera=frame.to_local(! _ray.direction);
        const int mat=intersection.solid->material;
        const Vector3 brdf = _world.materials[mat]->eval_brdf(intersection.point,
                                                             to_light,to_camera);
        
        Vector3 illum=(brdf*radiance)/(pdf);
        
        assert(! std::isnan(illum.x));
        assert(! std::isnan(illum.y));
        assert(! std::isnan(illum.z));
        assert(illum.x >= 0.f);
        assert(illum.y >= 0.f);
        assert(illum.z >= 0.f);
        
        illumination+=illum;
        
        assert(! std::isnan(illumination.x));
      }
      

      return illumination;
		}
		else
			return Vector3();
	}
}

