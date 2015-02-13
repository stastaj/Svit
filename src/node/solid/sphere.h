#ifndef SVIT_SPHERE
#define SVIT_SPHERE

#include "node/solid/solid.h"
#include "geom/point.h"
#include "geom/intersection.h"
#include "geom/ray.h"

#include <list>

namespace Svit
{
  class Sphere : public Solid
	{
		private:
			Point3 center;
			float radius;
      double radius_sqr;

		public:
      Sphere (Point3 _center, float _radius)
        : center(_center), radius(_radius),radius_sqr(_radius*_radius)
			{ }

			bool
      intersect (const Ray& _ray, Intersection& _intersection) override;

      AABB
      get_aabb() const override;

			void
			complete_intersection (Intersection &_intersection, const Ray& _ray) 
      const override;

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

