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
			get_ray (float _x, float _y) = 0;

			virtual void
			look_at (Point3 _dest) = 0;
	};
}

#endif
