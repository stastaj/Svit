#include "supersampling/random.h"

namespace Svit
{
  Vector2
	RandomSuperSampling::next_sample (int _x, int _y)
	{
		(void) _x;
		(void) _y;

    float x = distribution(generator);
    float y = distribution(generator);

		return Vector2(x, y);
	}

}

