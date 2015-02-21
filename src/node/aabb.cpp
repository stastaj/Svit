#include "node/aabb.h"
#include "math/constants.h"

#include <limits>

namespace Svit
{      
    bool
    AABB::intersect_with_line (const Ray& _ray, float& a, float& b) const {
      float t_min=-std::numeric_limits<float>::max();
      float t_max=std::numeric_limits<float>::max();
      const Vector3 p=center-_ray.origin;
      const Vector3 h=max-center;
      for(int i=0;i<3;++i){
        const float e=p[i];
        const float f=_ray.direction[i];
        const float f_inv=1.0f/f;
        if(std::abs(f)>EPS_DIVISION){
          float t1=(e+(h[i]))*f_inv;
          float t2=(e-(h[i]))*f_inv;
          if(t1>t2)
            std::swap(t1,t2);
          if(t1>t_min)
            t_min=t1;
          if(t2<t_max)
            t_max=t2;
          if(t_min>t_max)
            return false;         
        }    
        else if(-e-(h[i])>0 || -e+(h[i])<0)
          return false;
      }
      a=t_min;
      b=t_max;
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


