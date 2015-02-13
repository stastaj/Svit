#ifndef SVIT_AABB
#define SVIT_AABB

#include "geom/vector.h"

namespace Svit
{
  /**
   * @brief Axis Aligned Bounding Box.
   */
  class AABB
	{
		public:
      Vector3 min;
      Vector3 max;

      AABB(Vector3 _min, Vector3 _max) : min(_min),max(_max) {}
      
      float 
      surface_area()const {
        Vector3 tmp=max-min;
        return 2.0f*(tmp.x*tmp.y+tmp.x*tmp.z+tmp.y*tmp.z);
      }

	};

}

#endif

