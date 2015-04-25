#include "geom/aabb.h"
#include "math/constants.h"

#include <limits>
#include <cmath>

using std::min;
using std::max;

namespace Svit
{      
    bool
    AABB::intersect_with_line (const Ray& _ray, float& t_min, float& t_max) const {      
      const Vector3 f_inv=Vector3(1.0f,1.0f,1.0f)/_ray.direction;
      Vector3 e_plus_h=max-_ray.origin; // tmp_max
      Vector3 e_minus_h=min-_ray.origin; // tmp_min
      e_minus_h*=f_inv;
      e_plus_h*=f_inv;
      
#if defined(USE_SSE) && defined(__SSE__)
      Vector3 tmin(_mm_min_ps(e_minus_h.v,e_plus_h.v));
      Vector3 tmax(_mm_max_ps(e_minus_h.v,e_plus_h.v));
#else
      Vector3 tmin(min(e_minus_h.x,e_plus_h.x),
                   min(e_minus_h.y,e_plus_h.y),
                   min(e_minus_h.z,e_plus_h.z),
                   min(e_minus_h.w,e_plus_h.w));
      Vector3 tmax(max(e_minus_h.x,e_plus_h.x),
                   max(e_minus_h.y,e_plus_h.y),
                   max(e_minus_h.z,e_plus_h.z),
                   max(e_minus_h.w,e_plus_h.w));
#endif
      
      t_min=tmin.max();
      t_max=tmax.min();
      if(t_min>t_max)
        return false;    
      if(t_max<0)
        return false;
      return true;
    }
    
    float 
    AABB::surface_area() const {
      Vector3 tmp=max-min;
      return 2.0f*(tmp.x*tmp.y+tmp.x*tmp.z+tmp.y*tmp.z);
    }
    
    void 
    AABB::extend(AABB& _bb){
      for(int i=0;i<3;++i){
        min[i]=std::min(min[i],_bb.min[i]);
        max[i]=std::max(max[i],_bb.max[i]);
      }
      center=min+(max-min)*0.5f;
    }
}


