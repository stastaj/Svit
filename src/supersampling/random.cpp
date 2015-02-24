#include "supersampling/random.h"

namespace Svit
{
  void RandomSuperSampling::next_sample(Vector2& v)
	{
    v.x = distribution(generator);
    v.y = distribution(generator);
	}
  
  void RandomSuperSampling::next_sample3(Vector3& v)
	{
    v.x = distribution(generator);
    v.y = distribution(generator);
    v.z = distribution(generator);
	}
}

