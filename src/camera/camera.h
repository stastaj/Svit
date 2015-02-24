#ifndef SVIT_CAMERA
#define SVIT_CAMERA

#include "geom/point.h"
#include "geom/rect.h"
#include "geom/ray.h"

namespace Svit
{
	class Camera
	{
		public:
			virtual Ray 
      get_ray (const int _x, const int _y, const Vector2& _samples) const = 0;

			virtual void
			look_at (Point3 _dest) = 0;
	};
}

#endif
