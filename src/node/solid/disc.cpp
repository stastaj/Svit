#include "node/solid/disc.h"
#include "node/aabb.h"

namespace Svit
{
	Disc::Disc (Point3 _point, Vector3 _normal, float _radius)
	{
		point = _point;
		normal = _normal;
		radius = _radius;
		radius_sqr = radius * radius;
	}

	bool Disc::intersect(const Ray& _ray, Intersection& _intersection)
	{
		float angle = normal % _ray.direction;

		if (angle <= 0.0f)
			return false;

		float t = -(normal % (_ray.origin - point))/angle;
		if (t < _intersection.t && t > _ray.t_min)
		{
			Point3 hit_point = _ray(t);
			Vector3 difference = hit_point - point;

			if (difference % difference > radius_sqr)
				return false;	

			_intersection.t = t;
			_intersection.solid = this;

			return true;
		}
		else
			return false;
	}

	void
	Disc::complete_intersection (Intersection& _intersection, const Ray& _ray)
  const
	{
		_intersection.normal = normal;
    _intersection.point = _ray(_intersection.t);
	}

  AABB
  Disc::get_aabb() const {
    if(std::abs(normal.x) < 0.99f){
      Vector3 u(1.f,0.f,0.f);
      u=radius*~(u-(normal % u)*normal);
      Vector3 v=radius*~(normal & u);
      Vector3 p(normal.x,0.f,normal.z);
      p=~p;
      return AABB(Vector3(point.x-u.x,(p.x*u + p.z*v).y,point.z-v.z),
                  Vector3(point.x+u.x,(-p.x*u + -p.z*v).y,point.z+v.z));
    }
    else{
      Vector3 u(0.f,1.f,0.f);
      u=radius*~(u-(normal % u)*normal);
      Vector3 v=radius*~(normal & u);
      Vector3 p(0.f,normal.y,normal.z);
      p=~p;
      return AABB(Vector3((p.x*u + p.z*v).x,point.y-u.y,point.z-v.z),
                  Vector3((-p.x*u + -p.z*v).x,point.y+u.y,point.z+v.z));
    }
  }

	void
	Disc::dump (const char *_name, unsigned int _level)
	{
		std::cout << std::string(' ', _level*2) << _name << " = Disc" << std::endl;
		point.dump("point", _level+1);
		normal.dump("normal", _level+1);
		std::cout << std::string(' ', (_level+1)*2)  << "radius = " << radius <<
		    std::endl;
	}
}

