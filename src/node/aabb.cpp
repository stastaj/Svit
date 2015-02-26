#include "node/aabb.h"
#include "math/constants.h"

#include <limits>
#include <cmath>

namespace Svit
{      
    bool
    AABB::intersect_with_line (const Ray& _ray, float& t_min, float& t_max) const {      
      const Vector3 f_inv=Vector3(1.0f,1.0f,1.0f)/_ray.direction;
      Vector3 e_plus_h=max-_ray.origin; // tmp_max
      Vector3 e_minus_h=min-_ray.origin; // tmp_min
      e_minus_h*=f_inv;
      e_plus_h*=f_inv;
      
      Vector3 tmin(_mm_min_ps(e_minus_h.v,e_plus_h.v));
      Vector3 tmax(_mm_max_ps(e_minus_h.v,e_plus_h.v));
      
      t_min=tmin.max();
      t_max=tmax.min();
      if(t_min>t_max)
        return false;    
      if(std::signbit(t_max)) // t_max is negative
        return false;
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


