#ifndef SVIT_RANDOM_SUPERSAMPLING
#define SVIT_RANDOM_SUPERSAMPLING

#include "supersampling/supersampling.h"

#include <random>

namespace Svit
{
	class RandomSuperSampling : public SuperSampling
	{
		private:
      std::mt19937_64 generator;
			std::uniform_real_distribution<float> distribution;

		public:
      RandomSuperSampling (unsigned int _seed=1234):generator(_seed)
      {}
      
      float
			next_sample () override;
      
			Vector2
			next_sample2 () override;

      Vector3
			next_sample3 () override;

			SuperSampling*
      copy (unsigned int _seed)
			{
        RandomSuperSampling *c = new RandomSuperSampling(_seed);
        return c;
			}
	};
}

#endif

