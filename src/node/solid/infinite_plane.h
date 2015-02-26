#ifndef SVIT_INFINITE_PLANE
#define SVIT_INFINITE_PLANE

#include "node/solid/solid.h"
#include "node/node.h"
#include "node/aabb.h"
#include "geom/point.h"
#include "geom/vector.h"
#include "geom/ray.h"

namespace Svit
{
	class InfinitePlane : public Solid
	{
		private:
			Point3 point;
			Vector3 normal;

		public:
			InfinitePlane (Point3 _point, Vector3 _normal, int _mat, int _light)
				: Solid(_mat,_light), point(_point), normal(_normal)
			{ }

			bool
      intersect ( const Ray& _ray, Intersection& _intersection) const override;

      AABB
      get_aabb() const override;

			void
			dump (const char *_name, unsigned int _level = 0);
	};
}

#endif

