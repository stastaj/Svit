#include "light/point.h"
#include "math/frame.h"                 // for Frame

#include <cmath>                        // for sqrt

namespace Svit
{	
  Vector3 
  PointLight::sample_light (const Point3& _surface_point, const Frame& _frame, 
                         const Vector2& _samples, Vector3& _wig, 
                            float& _light_dist, float& _pdf) const 
	{
    (void)_samples;
    _pdf=1.f;
    _wig = position - _surface_point;
    float distance_sqr = _wig % _wig;
    _light_dist = std::sqrt(distance_sqr);
    _wig.normalize();
    float cos_theta = _frame.normal % _wig;
    if(cos_theta<0) 
      return Vector3();
    return intensity * cos_theta  / distance_sqr;
	}

  Vector3 
  PointLight::get_radiance (const Vector3& wig ) const 
	{
    (void)wig;
		return intensity; 
	}
  
  float 
  PointLight::get_pdf (const Vector3& _wig,float& _light_dist_sqr ) const 
	{
    (void)_wig;
    (void)_light_dist_sqr;
		return 0.f; 
	}
}

