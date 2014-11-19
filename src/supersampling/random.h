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

			Vector2
			next_sample (int _x, int _y);

      void
      add_result (Vector3 _result){}

      bool
      enough (){}

      Vector3
      final_result (){}

			SuperSampling*
      copy (unsigned int _seed)
			{
        RandomSuperSampling *c = new RandomSuperSampling(_seed);
        return c;
			}
	};
}

#endif

