#ifndef SVIT_RAY
#define SVIT_RAY

#include "geom/vector.h"
#include "geom/point.h"
#include "node/solid/solid.h"
#include "math/constants.h"

namespace Svit
{  
	class Ray
	{
		public:
			Point3 origin;
			Vector3 direction;
      float t_min; 
      unsigned int id;
      
			Ray (Point3 const& _origin, Vector3 const& _direction, unsigned int& _id,
           const float _f=RAY_EPSILON)
				: origin(_origin), direction(_direction), t_min(_f), id(++_id)
			{ }

      Ray (const Ray& _ray):origin(_ray.origin),direction(_ray.direction),
        t_min(_ray.t_min)
      {}
      
			Point3 
			operator() (float _time) const
			{
				return origin + (direction * _time);
			}

			void
			dump (const char *name, unsigned int level = 0)
			{
				std::string indentation(level*2, ' ');
				std::cout << indentation << name << " = Ray" << std::endl;
				origin.dump("origin", level+1);
				direction.dump("direction", level+1);
			}
	};
  
  

  
  
}

#endif

