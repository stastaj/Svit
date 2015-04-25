#ifndef SVIT_CAMERA
#define SVIT_CAMERA

#include "geom/point.h"
#include "geom/ray.h"

namespace Svit
{
  /**
   * @brief The Camera class represents the virtual camera - it drives how the 
   * rays are generated. 
   */
	class Camera
	{
		public:
      /**
       * @brief get_ray method generates new ray - its origin and direction -
       * which goes through specific pixel.
       * @param _x x-coordinate of the pixel through which the ray is created.
       * @param _y y-coordinate of the pixel through which the ray is created. 
       * @param _samples 2D random samples.
       * @return new ray object to be shot.
       */
			virtual Ray 
      get_ray (const int _x, const int _y, const Vector2& _samples, 
               unsigned int _id) const = 0;

			virtual void
			look_at (Point3 _dest) = 0;
	};
}

#endif
