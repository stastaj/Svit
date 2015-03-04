#ifndef SVIT_SUPERSAMPLING
#define SVIT_SUPERSAMPLING

#include "geom/vector.h"
	
namespace Svit
{
	class SuperSampling
	{
		public:
      
      virtual Vector3
			next_sample3 () = 0;
      
			virtual Vector2
			next_sample () = 0;

			virtual ~SuperSampling() { }

			virtual SuperSampling*
      copy ( unsigned int ) = 0;
	};
}

#endif

