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
			Disc (Point3 _point, Vector3 _normal, float _radius, int _mat, int _l):
        Solid(_mat,_l), point(_point),normal(_normal),radius(_radius),
                                    radius_sqr (_radius * _radius)
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

