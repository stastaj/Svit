#include "light/background.h"
#include "math/frame.h"                 // for Frame

#include <limits>                       
#include "math/numeric.h"

namespace Svit
{	
  Vector3 
  BackgroundLight::sample_light (const Point3& _surface_point, const Frame& _frame, 
                         const Vector2& _samples, Vector3& _wig, 
                            float& _light_dist, float& _pdf) const 
	{
    (void)_surface_point;
    _light_dist=std::numeric_limits<float>::max();
    _wig=_frame.to_world(sample_uniform_sphere_w(_samples,0));
    _pdf= INV_PI_F * 0.25f;
    return radiance;
	}

  Vector3 
  BackgroundLight::get_radiance (const Vector3& _wig ) const 
	{
    (void)_wig;
		return radiance; 
	}
  
  float 
  BackgroundLight::get_pdf(const Vector3& _wig,float& _light_dist_sqr) const 
	{
    (void)_wig;
    (void)_light_dist_sqr;
		return 0.25f * INV_PI_F;
	}
  
}

