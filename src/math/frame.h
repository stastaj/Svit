#ifndef FRAME_H
#define FRAME_H

#include "geom/vector.h"
#include "geom/operations.h"

namespace Svit
{
  /**
   * @brief The Frame class represents an orthonormal coordinate frame used for
   * conversions between coordinates - mainly global and tangent space. 
   */
  class Frame{
    public:
      Vector3 tangent, bitangent, normal;
      
      Frame( const Vector3& x, const Vector3& y, const Vector3& z):
        tangent(x),bitangent(y),normal(z)
      {}
      
      explicit Frame( const Vector3& z ){
        Vector3 tmpZ = normal = ~(z);
        Vector3 tmpX = (std::abs(tmpZ.x) > 0.99f) ? Vector3(0,1,0) : Vector3(1,0,0);
        bitangent = tmpZ & tmpX;
        bitangent.normalize_fast();
        tangent = bitangent & tmpZ;
      }
      
      /**
       * @brief to_world Converts given vector to global coordinates. 
       * @param _v vector to be converted. 
       * @return Converted vector in global coordinates. 
       */
      inline Vector3
      to_world(const Vector3& _v) const
      {
        return tangent * _v.x + bitangent * _v.y + normal * _v.z;
      }
      
      /**
       * @brief to_local Converts given vector to local coordinates. 
       * @param _v vector to be converted. 
       * @return Converted vector in local coordinates. 
       */
      inline Vector3
      to_local(const Vector3& _v) const
      {
        return Vector3 ( (_v % tangent), (_v % bitangent), (_v % normal));
      }
      
      
  };
}
#endif
