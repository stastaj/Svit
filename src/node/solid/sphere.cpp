#include "node/solid/sphere.h"

namespace Svit
{
	bool Sphere::intersect(const Ray& _ray, Intersection& _intersection)
	{
    // Real-time Rendering, 3rd edition, page 738

    Vector3 l=center-_ray.origin;
    float l_length_sqr=l % l;    
    float s=l % _ray.direction; 
    if(s < 0 && l_length_sqr > radius_sqr){
      return false;
    }
    float m_sqr=l_length_sqr-s*s;
    if(m_sqr > radius_sqr){
      return false;
    }
    // now the ray definitely intersect the sphere.
    float q=std::sqrt(radius_sqr-m_sqr);
    
    float t;
    if(l_length_sqr > radius_sqr){
      t=s-q;
    }
    else{
      t=s+q;
    }

    if( t <= _ray.t_min ||  t >= _intersection.t){
      return false;
    }
    _intersection.t = t;
    _intersection.node = this;
    
    return true;
  }

	void
  Sphere::complete_intersection (Intersection &_intersection, const Ray& _ray)
  const
	{
		_intersection.normal = ~(_intersection.point - center);
    _intersection.point = _ray(_intersection.t);
	}

  AABB
  Sphere::get_aabb() const {
    return AABB(center-radius,center+radius);
  }

	void
  Sphere::dump (const char *_name, unsigned int _level)
	{
		std::cout << std::string(' ', _level*2) << _name << " = Sphere" <<
		    std::endl;
		center.dump("center", _level+1);
		std::cout << std::string(' ', (_level+1)*2)  << "radius = " << radius <<
		    std::endl;
	}
}

