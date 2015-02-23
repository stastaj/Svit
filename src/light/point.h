#ifndef SVIT_POINT_LIGHT
#define SVIT_POINT_LIGHT

#include "light/light.h"
#include "geom/point.h"
#include "math/constants.h"

namespace Svit
{
	class PointLight : public Light
	{
		private:
			Point3 position;
			Vector3 intensity;
      Vector3 power;

		public:
      PointLight(Point3 _position, Vector3 _intensity):Light(Point),
        position(_position), intensity(_intensity), power(4.0f*PI_F*intensity)
      {}
			
      Vector3 
      sample_light ( const Point3& _surface_point, const Frame& _frame, 
                     const Vector2& _samples, Vector3& _wig, float& _light_dist,
                     float& _pdf) const override;

			Vector3
			get_radiance ( Vector3& wig ) const override;
	};
}

#endif

