#include "node/solid/sphere.h"

namespace Svit
{
	bool 
  Sphere::intersect(const Ray& _ray, Intersection& _intersection) const
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
    
    float t_min;//, t_max;
    if(l_length_sqr > radius_sqr){
      t_min=s-q;
      //t_max=s+q;
    }
    else{
      t_min=s+q;
      //t_max=s-q;
    }

    if( t_min > _ray.t_min &&  t_min < _intersection.t){
      _intersection.t = t_min;
      _intersection.point=_ray(t_min);
      _intersection.normal=~(_intersection.point - center);;
      _intersection.solid = (Solid*)this;
      return true;      
    }
    /*if( t_max > _ray.t_min &&  t_max < _intersection.t){
      _intersection.t = t_max;
      _intersection.solid = this;
      return true;      
    }*/
    return false;
  }

  AABB
  Sphere::get_aabb() const {
    return AABB(center-Vector3(radius,radius,radius),
                center+Vector3(radius,radius,radius));
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

