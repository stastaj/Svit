#ifndef SVIT_DISC
#define SVIT_DISC

#include "node/solid/solid.h"
#include "node/node.h"
#include "geom/point.h"
#include "geom/vector.h"
#include "geom/ray.h"

namespace Svit
{
	class Disc : public Solid
	{
		private:
			Point3 point;
			Vector3 normal;
			float radius;
			float radius2;

		public:
			Disc (Point3 _point, Vector3 _normal, float _radius);

			boost::optional<Intersection>
			intersect (Ray& _ray, float _best);

			void
			complete_intersection (Intersection *_intersection);

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

