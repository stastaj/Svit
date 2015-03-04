#ifndef SVIT_SPHERE
#define SVIT_SPHERE

#include "node/solid/solid.h"
#include "node/aabb.h"
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
      Sphere (Point3 _center, float _radius, int _mat, int _light=-1)
        : Solid(_mat, _light), center(_center), radius(_radius),
          radius_sqr(_radius*_radius)
			{ }

			bool
      intersect (const Ray& _ray, Intersection& _intersection) const override;

      AABB
      get_aabb() const override;

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

