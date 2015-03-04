#include "node/solid/triangle2.h"

namespace Svit
{
  Triangle2::Triangle2(Point3 _p0, Point3 _p1, Point3 _p2, int _mat, 
                     int _light): 
    Solid(_mat, _light), p0(_p0),p1(_p1),p2(_p2) 
  {
    e1=(p2-p0);
    e2=(p1-p0);
    normal=~(~e1 & ~e2);
  }
  
	bool Triangle2::intersect(const Ray& _ray, Intersection& _intersection) const
	{
    const Vector3 ao = p0 - _ray.origin;
    const Vector3 bo = p1 - _ray.origin;
    const Vector3 co = p2 - _ray.origin;

    const Vector3 v0 = co & bo;
    const Vector3 v1 = bo & ao;
    const Vector3 v2 = ao & co;

    const float v0d = v0 % _ray.direction;
    const float v1d = v1 % _ray.direction;
    const float v2d = v2 % _ray.direction;

    if(((v0d < 0.f)  && (v1d < 0.f)  && (v2d < 0.f)) ||
       ((v0d >= 0.f) && (v1d >= 0.f) && (v2d >= 0.f)))
    {
        const float t = (normal % ao) / (normal % _ray.direction);

        if((t > _ray.t_min) & (t < _intersection.t))
        {
          _intersection.t = t;
          _intersection.point = _ray(t);
          _intersection.normal=normal;
          _intersection.solid = (Solid*)this;
            return true;
        }
    }

    return false;
	}

  AABB
  Triangle2::get_aabb() const {
    return AABB( Vector3(std::min(std::min(p1.x,p2.x),p0.x),
                         std::min(std::min(p1.y,p2.y),p0.y),
                         std::min(std::min(p1.z,p2.z),p0.z)),
                 Vector3(std::max(std::max(p1.x,p2.x),p0.x),
                         std::max(std::max(p1.y,p2.y),p0.y),
                         std::max(std::max(p1.z,p2.z),p0.z))
          );
  }

	void
	Triangle2::dump (const char *_name, unsigned int _level)
	{
		std::cout << std::string(' ', _level*2) << _name << " = Triangle" << 
		    std::endl;

		p1.dump("p1", _level+1);
		(p1+e1).dump("p2", _level+1);
		(p1+e2).dump("p3", _level+1);
	}
}

