#include "node/aabb.h"
#include "math/constants.h"

#include <limits>
#include <cmath>

namespace Svit
{      
    bool
    AABB::intersect_with_line (const Ray& _ray, float& t_min, float& t_max) const {
      t_min=-std::numeric_limits<float>::max();
      t_max=std::numeric_limits<float>::max();
      
      //const Vector3 e_plus_h=max-_ray.origin;
      //const Vector3 e_minus_h=min-_ray.origin;
      //const Vector3 f_inv=Vector3(1.0f,1.0f,1.0f)/_ray.direction;
      
      for(int i=0;i<3;++i){
        const float e_plus_h=max[i]-_ray.origin[i];
        const float e_minus_h=min[i]-_ray.origin[i];
        if(std::abs(_ray.direction[i])>EPS_DIVISION){
          const float f_inv=1.0f/_ray.direction[i];
          float t1=(e_plus_h)*f_inv;
          float t2=(e_minus_h)*f_inv;
          if(t1>t2)
            std::swap(t1,t2);
          if(t1>t_min)
            t_min=t1;
          if(t2<t_max)
            t_max=t2;
          if(t_min>t_max)
            return false;    
          if(std::signbit(t_max)) // t_max is negative
            return false;
        }    
        else if(std::signbit(e_plus_h) || !std::signbit(e_minus_h))
          return false;
      }
      return true;
    }
    
    float 
    AABB::surface_area() const {
      Vector3 tmp=max-min;
      return 2.0f*(tmp.x*tmp.y+tmp.x*tmp.z+tmp.y*tmp.z);
    }
    
    void 
    AABB::update_to_contain(AABB& _bb){
      for(int i=0;i<3;++i){
        min[i]=std::min(min[i],_bb.min[i]);
        max[i]=std::max(max[i],_bb.max[i]);
      }
      center=min+(max-min)*0.5f;
    }
}


