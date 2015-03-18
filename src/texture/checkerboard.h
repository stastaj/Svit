#ifndef SVIT_CHECKERBOARD_TEXTURE
#define SVIT_CHECKERBOARD_TEXTURE

#include "texture/texture.h"

namespace Svit
{
	class CheckerboardTexture : public Texture
	{
		private:
			Vector3 black;
			Vector3 white;
			float size;
      float size_inv;

		public:
      CheckerboardTexture (Vector3 _black, Vector3 _white, float _size)
        :black(_black),white(_white),size(_size),size_inv(1.f/_size)
      {}

			Vector3
			get_color (const Point3& _point) const override;
	};
}

#endif

