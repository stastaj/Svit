#ifndef SVIT_PERSPECTIVE_CAMERA
#define SVIT_PERSPECTIVE_CAMERA

#include "camera/camera.h"
#include "geom/vector.h"
#include "geom/point.h"

namespace Svit
{
	class PerspectiveCamera : public Camera
	{
		private:
      float horizontal_angle; // in radians
			float aspect_ratio;
      Vector3 top_left_corner;
      Vector3 grid_diff_x;
      Vector3 grid_diff_y;

			void 
			recompute ();

		public:
			Point3 position;
			Vector3 up;
			Vector3 forward;
      Vector2i resolution;

      PerspectiveCamera (Point3 _position, Vector3 _forward, Vector3 _up,
          float _horizontal_angle, Vector2i _resolution);

			Ray
      get_ray (const int _x, const int _y, const Vector2& _samples) const override;

			void
      look_at (Point3 _dest) override;
	};
}

#endif

