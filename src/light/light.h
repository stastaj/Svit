#ifndef SVIT_LIGHT
#define SVIT_LIGHT

#include "geom/vector.h"
#include "geom/point.h"
#include "math/frame.h"

namespace Svit
{
  enum LightType {Point,Area,Background};

	class Light 
	{
    protected:
      Light(const LightType _type):type(_type){}
		
    public:      
      const LightType type;
      
			virtual Vector3 
      sample_light ( const Point3& _surface_point, const Frame& _frame, 
                     const Vector2& _samples, Vector3& _wig, float& _light_dist,
                     float& _pdf) const = 0;
      
      //virtual
      //Vector3 get_power() = 0;
      
			virtual Vector3 
      get_radiance ( const Vector3& wig ) const = 0;
	};
}

#endif

