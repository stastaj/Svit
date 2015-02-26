#ifndef SVIT_PERLIN_NOISE_TEXTURE
#define SVIT_PERLIN_NOISE_TEXTURE

#include "texture/texture.h"

#include <vector>

namespace Svit
{
	class PerlinNoiseTexture : public Texture
	{
		protected:
			const Vector3 black;
			const Vector3 white;
			unsigned int octave_count;
			std::vector<float> amplitudes;
			std::vector<float> frequencies;

			float
			noise (int _x, int _y, int _z) const;

			float
			noise_3D (const Point3& _point) const;

		public:
      PerlinNoiseTexture (Vector3 _black, Vector3 _white):
          black(_black),white(_white),octave_count(0)
      {}

			void
			add_octave (float _amplitude, float _frequency);

			virtual Vector3
			get_color (const Point3& _point) const override;
	};
}

#endif

