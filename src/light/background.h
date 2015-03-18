#ifndef SVIT_BACKGROUND_LIGHT
#define SVIT_BACKGROUND_LIGHT

#include "light/light.h"
#include "geom/point.h"
#include "math/constants.h"
#include "geom/vector.h"                // for Vector3, Vector2

namespace Svit
{
  class Frame;
  
	class BackgroundLight : public Light
	{
		private:
			Vector3 radiance;
      
		public:
      BackgroundLight(Vector3 _radiance = (Vector3(135, 206, 250)/
                                          Vector3(255.f,255.f,255.f)))
        :Light(Background),radiance(_radiance)
      {}
			
      Vector3 
      sample_light ( const Point3& _surface_point, const Frame& _frame, 
                     const Vector2& _samples, Vector3& _wig, float& _light_dist,
                     float& _pdf ) const override;

			Vector3
			get_radiance ( const Vector3& _wig ) const override;
      
      float 
      get_pdf ( const Vector3& _wig,float& _light_dist_sqr ) const override;
	};
}

#endif

