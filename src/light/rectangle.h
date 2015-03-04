#ifndef SVIT_RECTANGLE_LIGHT
#define SVIT_RECTANGLE_LIGHT

#include "light/light.h"
#include "geom/point.h"
#include "math/constants.h"

namespace Svit
{
	class RectangleLight : public Light
	{
		private:
			Vector3 radiance;
      Vector3 power;
      Point3 p0;
      Vector3 e1,e2;
      Vector3 normal;
      float area_inv;
      
		public:
      RectangleLight(Point3 _p0, Point3 _p1, Point3 _p2,Vector3 _radiance):
        Light(Area), p0(_p0),radiance(_radiance),e1(_p1 - _p0), e2(_p2 - _p0),
        normal(~(e1 & e2))
      {
        float area = std::sqrt((e2 & e1) % (e2 & e1));
        area_inv = 1.0f / area;
        power=area*radiance*PI_F;
      }
			
      Vector3 
      sample_light (const Point3& _surface_point, const Frame& _frame, 
      const Vector2& _samples, Vector3& _wig, float& _light_dist,
      float& _pdf) const override;

			Vector3
			get_radiance ( const Vector3& wig ) const override;
	};
}

#endif

