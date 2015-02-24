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

			void
			next_sample (Vector2& v) override;

      void
			next_sample3 (Vector3& v) override;

			SuperSampling*
      copy (unsigned int _seed)
			{
        RandomSuperSampling *c = new RandomSuperSampling(_seed);
        return c;
			}
	};
}

#endif

