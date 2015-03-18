#include "texture/checkerboard.h"

#include <cmath>

namespace Svit 
{
	Vector3
	CheckerboardTexture::get_color (const Point3& _point) const
	{
		bool x = (int)floor(_point.x * size_inv) % 2 == 0;
		bool y = (int)floor(_point.y * size_inv) % 2 == 0;
		bool z = (int)floor(_point.z * size_inv) % 2 == 0;

		return (x == y == z ? black : white);
	}
}

