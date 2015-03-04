#include "supersampling/random.h"

namespace Svit
{
  Vector2 RandomSuperSampling::next_sample()
	{
    return Vector3(distribution(generator),
                   distribution(generator));
	}
  
  Vector3 RandomSuperSampling::next_sample3()
	{
    return Vector3(distribution(generator),
                   distribution(generator),
                   distribution(generator));
	}
}

