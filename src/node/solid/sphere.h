#ifndef SVIT_SPHERE
#define SVIT_SPHERE

#include "node/solid/solid.h"
#include "geom/point.h"

#include <list>

namespace Svit
{
	class Sphere : public Solid
	{
		private:
			Point3 center;
			float radius;

		public:
			Sphere (Point3 _center, float _radius)
				: center(_center), radius(_radius)
			{ }

			boost::optional<Intersection>
      intersect (const Ray& _ray, const float _best);

			void
			complete_intersection (Intersection *_intersection);

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

