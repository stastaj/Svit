#ifndef SVIT_TRIANGLE
#define SVIT_TRIANGLE

#include "node/solid/solid.h"
#include "geom/point.h"
#include "geom/vector.h"
#include "geom/intersection.h"
#include "geom/ray.h"
#include "node/aabb.h"

#include <list>

namespace Svit
{
	class Triangle : public Solid
	{
		private:
			Vector3 normal;			
      Vector3 e1;
      Vector3 e2;
      Point3 p1;
      
		public:
			

			Triangle (Point3 _p1, Point3 _p2, Point3 _p3, int _mat, int _light)
        : Solid(_mat, _light), p1(_p1), e1(_p2 - _p1),e2(_p3 - _p1), 
          normal(~(e1 & e2))
      {}
        

			bool
      intersect (const Ray& _ray, Intersection& _intersection) const override;

      AABB
      get_aabb() const override;

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

