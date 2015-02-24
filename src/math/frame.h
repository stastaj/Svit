#ifndef FRAME_H
#define FRAME_H

#include "geom/vector.h"
#include "geom/operations.h"

namespace Svit
{
  class Frame{
    public:
      Vector3 mX, mY, mZ;
      
      Frame( const Vector3& x, const Vector3& y, const Vector3& z):
        mX(x),mY(y),mZ(z)
      {}
      
      explicit Frame( const Vector3& z ){
        Vector3 tmpZ = mZ = ~(z);
        Vector3 tmpX = (std::abs(tmpZ.x) > 0.99f) ? Vector3(0,1,0) : Vector3(1,0,0);
        mY = tmpZ & tmpX;
        mY.normalize_fast();
        mX = mY & tmpZ;
      }
      /*
    void set_from_z(const Vector3& z)
    {
      Vector3 tmpZ = mZ = ~(z);
      Vector3 tmpX = (std::abs(tmpZ.x) > 0.99f) ? Vector3(0,1,0) : Vector3(1,0,0);
      mY = ~ (* & (tmpZ, tmpX) );
      mX = * & (mY, tmpZ);
    }*/
      
      inline Vector3
      to_world(const Vector3& a) const
      {
        return mX * a.x + mY * a.y + mZ * a.z;
      }
      
      inline Vector3
      to_local(const Vector3& a) const
      {
        return Vector3 ( (a % mX), (a % mY), (a % mZ));
      }
      
      
  };
}
#endif
