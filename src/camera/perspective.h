#ifndef SVIT_PERSPECTIVE_CAMERA
#define SVIT_PERSPECTIVE_CAMERA

#include "camera/camera.h"
#include "geom/vector.h"
#include "geom/point.h"

namespace Svit
{
  /**
   * @brief The PerspectiveCamera class represents the most simple perspective 
   * camera with no film plane - the pinhole camera. Film plane cannot be 
   * intersected. All the rays have common origin. The image (film) plane 
   * is thought to be 1 unit in front of the camera position. 
   * 
   */
	class PerspectiveCamera : public Camera
	{
		private:
      float horizontal_angle; // in radians
			float aspect_ratio; // of the image plane
      Vector3 top_left_corner; // of the image plane
      Vector3 grid_diff_x; // between adjacent pixels
      Vector3 grid_diff_y; // between adjacent pixels

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
      get_ray (const int _x, const int _y, const Vector2& _samples, 
               unsigned int _id) const override;

			void
      look_at (Point3 _dest) override;
	};
}

#endif

