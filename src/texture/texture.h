#ifndef SVIT_TEXTURE
#define SVIT_TEXTURE

#include "geom/vector.h"
#include "geom/point.h"

namespace Svit
{
  /**
   * @brief The Texture class represents one texture of arbitraly dimension. 
   */
	class Texture
	{
		public:
      /**
       * @brief get_color for given surface point returns the texture color. 
       * @param _point
       * @return for given surface point returns the texture color. 
       */
			virtual Vector3
			get_color (const Point3& _point) const = 0;
	};
}

#endif

