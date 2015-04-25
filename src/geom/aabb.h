#ifndef SVIT_AABB
#define SVIT_AABB

#include "geom/vector.h"
#include "geom/ray.h"

#include <memory>

namespace Svit
{
  /**
   * @brief Axis Aligned Bounding Box. 
   */
  class AABB
	{
    private:
      Vector3 center;
		public:
      Vector3 min;
      Vector3 max;
      
      AABB(Vector3 _min, Vector3 _max) : min(_min),max(_max),
        center(min+(max-min)*0.5f)
      {}
      
      bool
      intersect_with_line (const Ray& _ray, float& a, float& b) const ;
            
      float 
      surface_area()const ;
      
      void 
      extend(AABB& _bb);

	};

}

#endif

