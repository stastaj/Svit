#include "texture/constant.h"

namespace Svit 
{
	ConstantTexture::ConstantTexture (Vector3 _color)
	{
		color = _color;
	}

	Vector3
	ConstantTexture::get_color (const Point3& _point) const
	{
		(void) _point;
		return color;
	}
}

