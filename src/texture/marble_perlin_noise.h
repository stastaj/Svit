#ifndef SVIT_MARBLE_PERLIN_NOISE_TEXTURE
#define SVIT_MARBLE_PERLIN_NOISE_TEXTURE

#include "texture/perlin_noise.h"

namespace Svit
{
	class MarblePerlinNoiseTexture : public PerlinNoiseTexture
	{
		public:
			MarblePerlinNoiseTexture (Vector3 _black, Vector3 _white) : 
        PerlinNoiseTexture(_black, _white)
      {}	

			Vector3
			get_color (const Point3& _point) const override;
	};
}

#endif

