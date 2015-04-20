#include "camera/perspective.h"

namespace Svit
{
  PerspectiveCamera::PerspectiveCamera (Point3 _position, Vector3 _forward,
      Vector3 _up, float _horizontal_angle, Vector2i _resolution)
		: position(_position), 
			forward(~_forward), 
			up(~_up), 
      horizontal_angle(_horizontal_angle),
      resolution(_resolution)
	{
		recompute();
	}

	void 
	PerspectiveCamera::recompute ()
	{
		Vector3 left = ~(forward & up);
    float half_width = std::tan(horizontal_angle * 0.5f);
    aspect_ratio=(float)resolution.y / (float)resolution.x;
    float half_height = half_width * aspect_ratio;

    top_left_corner=forward+left*half_width+half_height*up;
    grid_diff_x=((half_width*2.0f)/(float)resolution.x)*(!left);
    grid_diff_y=((half_width*2.0f)/(float)resolution.x)*(!up);
	}

	Ray
  PerspectiveCamera::get_ray (const int _x, const int _y, const Vector2& _samples)
  const
	{
		return Ray (position, ~(top_left_corner+(_x+_samples.x)*grid_diff_x+
                                            (_y+_samples.y)*grid_diff_y));		
	}

	void
	PerspectiveCamera::look_at (Point3 _dest)
	{
    /* Must also change up vector!
		forward = ~(_dest - position);
    recompute();
    */
	}
}

