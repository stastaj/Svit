#include "node/aabb.h"
#include "math/constants.h"

#include <limits>
#include <cmath>

namespace Svit
{      
    bool
    AABB::intersect_with_line (const Ray& _ray, float& t_min, float& t_max) const {      
      const Vector3 f_inv=Vector3(1.0f,1.0f,1.0f)/_ray.direction;
      const Vector3 e_plus_h=max-_ray.origin;
      const Vector3 e_minus_h=min-_ray.origin;
      Vector3 tmp_min=e_minus_h*f_inv;
      Vector3 tmp_max=e_plus_h*f_inv;
      
      t_min=std::max(std::max(std::min(tmp_min.x, tmp_max.x),
                              std::min(tmp_min.y, tmp_max.y)), 
                     std::min(tmp_min.z, tmp_max.z));
      t_max=std::min(std::min(std::max(tmp_min.x, tmp_max.x),
                              std::max(tmp_min.y, tmp_max.y)), 
                     std::max(tmp_min.z, tmp_max.z));
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


