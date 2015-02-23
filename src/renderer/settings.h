#ifndef SVIT_SETTINGS
#define SVIT_SETTINGS

#include "geom/vector.h"

namespace Svit
{
	struct Settings
	{
    Vector2i resolution;
		unsigned int max_thread_count;
    unsigned int iterations;
    unsigned int time;

		void
		dump (const char *name, unsigned int level = 0)
		{
			std::string indentation(level*2, ' ');	
			std::cout << indentation << name << " = Settings" << std::endl;
      resolution.dump("resolution", level+1);
			// TODO sample_count and max_thread_count
		}
	};
}

#endif

