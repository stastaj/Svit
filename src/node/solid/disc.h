#ifndef SVIT_DISC
#define SVIT_DISC

#include "node/solid/solid.h"
#include "geom/point.h"
#include "geom/vector.h"

namespace Svit
{
	class Disc : public Solid
	{
		private:
			Point3 point;
			Vector3 normal;
			float radius;
			float radius_sqr;

		public:
			Disc (Point3 _point, Vector3 _normal, float _radius);

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

