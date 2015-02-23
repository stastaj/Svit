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
      
      
      Vector3 illumination(0.0f,0.0f,0.0f);
      for (auto &light : _world.lights)
      {
        /*
        const LightHit light_hit = light->get_light_hit(intersection.point);
        
        if(light->type!=Background){
          const Point3 shadow_point = intersection.point+light_hit.direction*RAY_EPSILON;
          const Ray shadow_ray(shadow_point, light_hit.direction, 0);
          
          Intersection shadow_isect(light_hit.distance);
          if (_world.scene->intersect(shadow_ray, shadow_isect))
            continue;
        }
        Vector3 light_component = light->get_intensity(light_hit);
        */
        Vector3 wig;
        float pdf;
        float dist_sqr;
        const Vector2 samples;
        
        
        const Frame frame(intersection.normal);
        const Vector3 radiance=light->sample_light(intersection.point,frame,
                                                   samples,wig,dist_sqr,pdf);
        
        const Ray shadow_ray(intersection.point+wig*RAY_EPSILON, wig, 0);
        Intersection shadow_isect(dist_sqr-2.0f*RAY_EPSILON);
        if (_world.scene->intersect(shadow_ray, shadow_isect))
            continue;
        const Vector3 to_light=frame.to_local(wig);
        const Vector3 to_camera=frame.to_local(! _ray.direction);
        const Vector3 brdf = intersection.solid->material->eval_brdf(intersection.point,
                                                             to_light,to_camera);
        
        illumination+=(brdf*radiance)/(pdf);
      }
      

      return illumination;
		}
		else
			return Vector3();
	}
}

