#ifndef SVIT_INTERSECTION
#define SVIT_INTERSECTION

#include "geom/vector.h"
#include "geom/point.h"

namespace Svit
{   
  class Solid;
  
  class Intersection
	{      
    public:
      const Solid *solid;	
      float t; // distance to the closest intersection - serves as t_max
      Vector3 normal;
      Point3 point; // intersection point coordinates
      short material;
      short light;
      
      explicit Intersection(const float _t_max):t(_t_max),material(-1),
        light(-1),normal(Vector3()),point(Vector3()),solid(0) 
      {}
      
      Intersection(const Intersection& _i):t(_i.t),material(_i.material),
        light(_i.light),solid(_i.solid)
      {}
      
	};
  
}

#endif

