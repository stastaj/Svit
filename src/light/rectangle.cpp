#include "light/rectangle.h"

namespace Svit
{	
  Vector3 
  RectangleLight::sample_light (const Point3& _surface_point, 
                                const Frame& _frame, const Vector2& _samples, 
                                Vector3& _wig, float& _light_dist,
  float& _pdf) const
	{
    _pdf=area_inv;
    /*if(oLightDist==-0){
      return mRadiance ;
    }*/
    Vector3 light_pos=p0+ (e1*_samples.x) + (e2*_samples.y); 
    _wig = light_pos - _surface_point;
    _light_dist = _wig % _wig;
    _wig *= 1.0f / std::sqrt(_light_dist);
    float cos_thetaY=!_wig % normal;
    float cos_thetaX=_frame.mZ % _wig;
    if(cos_thetaY < 0 || cos_thetaX < 0){
      return Vector3();
    }
    return (cos_thetaX*radiance);
	}

  Vector3 
  RectangleLight::get_radiance ( Vector3& wig ) const 
	{
		return radiance; 
	}
}

