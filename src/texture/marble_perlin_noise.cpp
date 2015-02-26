#include "texture/marble_perlin_noise.h"
#include "math/interpolation.h"

namespace Svit
{
	Vector3
	MarblePerlinNoiseTexture::get_color (const Point3& _point) const
	{
		float sum = 0.0;
		float discard;
		for (unsigned int i = 0; i < octave_count; i++)
			sum += fabs(noise_3D(_point * frequencies[i])) * amplitudes[i];

		sum = sinf(_point.x + sum);

		if (sum > 1.0f)
			sum = 1.0f;

		if (sum < 0.0f)
			sum = 0.0f;

		return lerp_1D<float>(black, white, sum);
	}
}

