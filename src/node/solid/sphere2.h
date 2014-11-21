#ifndef SVIT_SPHERE2
#define SVIT_SPHERE2

#include "node/solid/solid.h"
#include "geom/point.h"

#include <list>

namespace Svit
{
  class Sphere2 : public Solid
	{
		private:
			Point3 center;
			float radius;

		public:
      Sphere2 (Point3 _center, float _radius)
				: center(_center), radius(_radius)
			{ }

			boost::optional<Intersection>
			intersect (Ray& _ray, float _best);

			void
			complete_intersection (Intersection *_intersection);

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

