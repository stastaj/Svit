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
			virtual void
			render (World& _world, Settings& _settings, Engine& _engine,
      SuperSampling* _super_sampling, Image& _final_image) = 0;
	};
}

#endif

