#ifndef SVIT_ENGINE
#define SVIT_ENGINE

#include "geom/ray.h"
#include "world/world.h"
#include "node/node.h"
#include "node/solid/solid.h"
#include "supersampling/supersampling.h"

#include <list>
#include <boost/optional.hpp>

namespace Svit
{
	class Engine
	{
		public:
			virtual Vector3 
      get_color (const Ray& _ray, const World& _world, SuperSampling* _sampler) const = 0;
      
      bool
      is_occluded(const World& _world, const Vector3& _point, float _distance, 
                  const Vector3& _direction) const {
        const Ray shadow_ray(_point+_direction*RAY_EPSILON, _direction, 0);
        Intersection shadow_isect(_distance-2.0f*RAY_EPSILON);
        return _world.scene->intersect(shadow_ray, shadow_isect);
      }
	};
}

#endif

