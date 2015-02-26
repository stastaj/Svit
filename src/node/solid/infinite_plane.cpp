#include "node/solid/infinite_plane.h"

#include <limits>
#include <iostream>
#include <string>

namespace Svit
{
	bool InfinitePlane::intersect(const Ray& _ray, Intersection& _intersection) 
  const
	{
		const float angle = normal % _ray.direction;

		if (std::abs(angle) <= EPS_DIVISION)
			return false;

		const float t = (normal % (point - _ray.origin))/angle;
		
    if(t <= _ray.t_min)
      return false;
    if( t>=_intersection.t)
      return false;
    
    _intersection.t = t;
    _intersection.point=_ray(t);
    _intersection.normal=normal;
    _intersection.solid = (Solid*)this;
    return true;
	}

  AABB
  InfinitePlane::get_aabb() const {
    return AABB(Vector3(-std::numeric_limits<float>::infinity(),
                        -std::numeric_limits<float>::infinity()
                        -std::numeric_limits<float>::infinity()),
                Vector3(std::numeric_limits<float>::infinity(),
                        std::numeric_limits<float>::infinity(),
                        std::numeric_limits<float>::infinity()));
  }

	void
	InfinitePlane::dump (const char *_name, unsigned int _level)
	{
		std::cout << std::string(' ', _level*2) << _name << " = Infinite Plane" <<
		    std::endl;
		point.dump("point", _level+1);
		normal.dump("normal", _level+1);
	}
}

