#ifndef SVIT_TRIANGLE2
#define SVIT_TRIANGLE2

#include "node/solid/solid.h"
#include "geom/point.h"
#include "geom/vector.h"
#include "geom/intersection.h"
#include "geom/ray.h"
#include "node/aabb.h"

#include <list>

namespace Svit
{
	class Triangle2 : public Solid
	{
		private:
			Vector3 normal;			
      Vector3 e1;
      Vector3 e2;
      Point3 p0, p1, p2;
      
		public:

			Triangle2 (Point3 _p0, Point3 _p1, Point3 _p2, int _mat, int _light=-1);

			bool
      intersect (const Ray& _ray, Intersection& _intersection) const override;

      AABB
      get_aabb() const override;

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

