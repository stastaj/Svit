#include "engine/path_tracing.h"
#include "light/light.h"
#include "math/numeric.h"
#include "math/frame.h"

#include <numeric>

namespace Svit
{
	Vector3
	RayTracingEngine::get_color (Ray& _ray, World& _world)
	{
		boost::optional<Intersection> best = _world.scene->intersect(_ray,
		    std::numeric_limits<float>::max());

		if (best)
		{
      Vector3 color(0,0,0);
			Intersection i = *best;
			best->node->complete_intersection(&i);

			Vector3 light_component(0.0, 0.0, 0.0);
			for (auto &light : _world.lights)
			{
				LightHit light_hit = light->get_light_hit(i.point);

				Point3 shadow_point = i.point + (light_hit.direction * 0.0001f);
				Ray shadow_ray(shadow_point, light_hit.direction);

				boost::optional<Intersection> shadow =
				    _world.scene->intersect(shadow_ray, light_hit.distance - 0.0001f);

				if (shadow)
					continue;

				light_component += light->get_intensity(light_hit);

        Frame frame;
        frame.set_from_z(i.normal);

        Vector3 to_light=frame.to_local(light_hit.direction);
        Vector3 to_camera=frame.to_local(! _ray.direction);
        Vector3 material_component = i.node->material->eval_brdf(i.point,
        to_light,to_camera);

        color=color+material_component*light_component;
			}


      return color;
		}
		else
			return Vector3();
	}
}

