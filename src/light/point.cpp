#include "light/point.h"

#include <cmath>

namespace Svit
{	
	LightHit 
	PointLight::get_light_hit (Point3 _point)
	{
		LightHit light_hit;
		light_hit.direction = position - _point;
    light_hit.distance = light_hit.direction % light_hit.direction;
    light_hit.direction.normalize();
		

		return light_hit;
	}

	Vector3
	PointLight::get_intensity (LightHit _hit)
	{
		return intensity; 
	}
}

