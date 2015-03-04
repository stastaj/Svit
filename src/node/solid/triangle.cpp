#include "node/solid/triangle.h"

namespace Svit
{
  Triangle::Triangle(Point3 _p1, Point3 _p2, Point3 _p3, int _mat, 
                     int _light): 
    Solid(_mat, _light), p1(_p1),p2(_p2),p3(_p3) 
  {
    e1=(p3-p1);
    e2=(p2-p1);
    normal=~(e1 & e2);
  }
  
	bool Triangle::intersect(const Ray& _ray, Intersection& _intersection) const
	{
		Vector3 P = _ray.direction & e2;
		float det = e1 % P;
		if (std::abs(det) <= EPS_DIVISION ) 
			false;
		float inv_det = 1.0f / det;

		Vector3 T = _ray.origin - p1;
		float u = (T % P) * inv_det;
		if (u < 0.f || u > 1.f) 
			return false;

		Vector3 Q = T & e1;
		float v = (_ray.direction % Q) * inv_det;
		if (v < 0.f || u + v  > 1.f) 
			return false;
																												  
		float t = (e2 % Q) * inv_det;
		if(t > _ray.t_min && t < _intersection.t)
		{ 
			_intersection.t = t;
      _intersection.point = _ray(t);
      _intersection.normal=normal;
			_intersection.solid = (Solid*)this;

			return true;
		}

		return false;
	}

  AABB
  Triangle::get_aabb() const {
    return AABB( Vector3(std::min(std::min(p1.x,p2.x),p3.x),
                         std::min(std::min(p1.y,p2.y),p3.y),
                         std::min(std::min(p1.z,p2.z),p3.z)),
                 Vector3(std::max(std::max(p1.x,p2.x),p3.x),
                         std::max(std::max(p1.y,p2.y),p3.y),
                         std::max(std::max(p1.z,p2.z),p3.z))
          );
  }

	void
	Triangle::dump (const char *_name, unsigned int _level)
	{
		std::cout << std::string(' ', _level*2) << _name << " = Triangle" << 
		    std::endl;

		p1.dump("p1", _level+1);
		(p1+e1).dump("p2", _level+1);
		(p1+e2).dump("p3", _level+1);
	}
}

