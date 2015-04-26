#ifndef SVIT_RENDERER
#define SVIT_RENDERER

#include "image/image.h"
#include "world/world.h"
#include "engine/engine.h"
#include "renderer/settings.h"
#include "supersampling/supersampling.h"

namespace Svit
{
	class Renderer
	{
		public:
      /**
       * @brief render method is called once when the light simulation is 
       * started. For given scene - _world, _settings, _sampler and integrating
       * engine _engine - computes incoming radiance to the camera and stores 
       * the final image to the _final_image.
       * @param _world input scene description.
       * @param _settings render settings.
       * @param _engine light integrator to be used.
       * @param _super_sampling sampler for random number generation. 
       * @param _final_image where the final computed image is stored. 
       */
			virtual void
			render (World& _world, Settings& _settings, Engine& _engine,
      SuperSampling* _super_sampling, Image& _final_image) = 0;
	};
}

#endif

