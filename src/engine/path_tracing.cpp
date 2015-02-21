#include "engine/path_tracing.h"

#include <numeric>

namespace Svit
{
	Vector3
  PathTracing::get_color (const Ray& _ray, const World& _world) const
	{
    Intersection intersection(std::numeric_limits<float>::max());
    if(_world.scene->intersect(_ray,intersection)){
      intersection.solid->complete_intersection(intersection,_ray);
      
      
      Vector3 color(0.0f,0.0f,0.0f);
      for (auto &light : _world.lights)
      {
       
        
        const LightHit light_hit = light->get_light_hit(intersection.point);
        
        if(light->type!=Background){
          const Point3 shadow_point = intersection.point+light_hit.direction*RAY_EPSILON;
          const Ray shadow_ray(shadow_point, light_hit.direction, 0);
          
          Intersection shadow_isect(light_hit.distance);
          if (_world.scene->intersect(shadow_ray, shadow_isect))
            continue;
        }
        Vector3 light_component = light->get_intensity(light_hit);
        
        Frame frame(intersection.normal);
        
        Vector3 to_light=frame.to_local(light_hit.direction);
        Vector3 to_camera=frame.to_local(! _ray.direction);
        Vector3 mat = intersection.solid->material->eval_brdf(intersection.point,
                                                             to_light,to_camera);
        
        color+=mat*light_component;
      }
      

      return color;
		}
		else
			return Vector3();
	}
}

