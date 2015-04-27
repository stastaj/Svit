#ifndef SVIT_INTERSECTION
#define SVIT_INTERSECTION

#include "geom/vector.h"
#include "geom/point.h"

namespace Svit
{   
  class Solid;
  
  /**
   * @brief The Intersection class stores all necessary info about an 
   * intersection.
   */
  class Intersection
	{      
    public:
      /**
       * @brief solid pointer to intersected solid. 
       */
      Solid *solid;	
      /**
       * @brief t time parameter, when the ray intersected the solid. Also 
       * distance to the closest intersection. Also serves as maximal distance
       * for finding an intersection.
       */
      float t; 
      /**
       * @brief normal vector at the intersected point. 
       */
      Vector3 normal;
      /**
       * @brief point 3d coordinates of the intersection. 
       */
      Point3 point; 
      
      explicit Intersection(const float _t_max):
		solid(0),
        t(_t_max),
        normal(Vector3()),
        point(Vector3())
      {}
      
      Intersection(const Intersection& _i):solid(_i.solid),t(_i.t)
      {}
      
	};
  
}

#endif

