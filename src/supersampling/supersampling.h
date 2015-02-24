#ifndef SVIT_SUPERSAMPLING
#define SVIT_SUPERSAMPLING

#include "geom/vector.h"
	
namespace Svit
{
	class SuperSampling
	{
		public:
      
      virtual void
			next_sample3 (Vector3& v) = 0;
      
			virtual void
			next_sample (Vector2& v) = 0;

			virtual ~SuperSampling() { }

			virtual SuperSampling*
      copy ( unsigned int ) = 0;
	};
}

#endif

