#ifndef SVIT_POINT_LIGHT
#define SVIT_POINT_LIGHT

#include "light/light.h"
#include "geom/point.h"
#include "math/constants.h"
#include "geom/vector.h"                // for Vector3, Vector2

namespace Svit
{
  class Frame;
  
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
			get_radiance ( const Vector3& wig ) const override;
      
      float 
      get_pdf(const Vector3& _wig,float& _light_dist_sqr) const;
	};
}

#endif

