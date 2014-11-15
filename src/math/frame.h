#ifndef FRAME_H
#define FRAME_H

#include "geom/vector.h"
#include "geom/operations.h"

namespace Svit
{
	class Frame{
	public:

		Frame():mX(Vector3(1.f,0.f,0.f)),mY(Vector3(0.f,1.f,0.f)),mZ(Vector3(0.f,0.f,1.f)){}

		Frame( const Vector3& x, const Vector3& y, const Vector3& z):mX(x),mY(y),mZ(z) {}

    void set_from_z(const Vector3& z)
		{
			Vector3 tmpZ = mZ = ~(z);
			Vector3 tmpX = (std::abs(tmpZ.x) > 0.99f) ? Vector3(0,1,0) : Vector3(1,0,0);
			mY = ~ (* & (tmpZ, tmpX) );
			mX = * & (mY, tmpZ);
		}

    Vector3 to_world(const Vector3& a) const
		{
			return mX * a.x + mY * a.y + mZ * a.z;
		}

    Vector3 to_local(const Vector3& a) const
		{
			return Vector3 ( (a % mX), (a % mY), (a % mZ));
		}

	public:
		Vector3 mX, mY, mZ;
	};
}
#endif
