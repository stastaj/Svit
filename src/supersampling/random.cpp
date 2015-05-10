#include "supersampling/random.h"

namespace Svit
{
  float RandomSuperSampling::next_sample()
	{
    return distribution(generator);
	}
  
  Vector2 RandomSuperSampling::next_sample2()
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

