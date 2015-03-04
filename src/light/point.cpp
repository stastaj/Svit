#include "light/point.h"

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
    _wig.normalize_fast();
    float cos_theta = _frame.mZ % _wig;
    if(std::signbit(cos_theta)) // cos_theta is negative
      return Vector3();
    return intensity * cos_theta  / distance_sqr;
	}

  Vector3 
  PointLight::get_radiance (const Vector3& wig ) const 
	{
    (void)wig;
		return intensity; 
	}
}

