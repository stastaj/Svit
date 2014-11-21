#ifndef SVIT_PARALLEL_RENDERER
#define SVIT_PARALLEL_RENDERER

#include "renderer/settings.h"
#include "image/image.h"
#include "renderer/parallel/task_dispatcher.h"
#include "geom/rect.h"
#include "world/world.h"
#include "engine/engine.h"
#include "renderer/renderer.h"

#include <signal.h>
#include <vector>
#include <utility>
#include <string.h>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/system/error_code.hpp>


namespace Svit
{
	struct Tile
	{
		Task task;
		Image image;
	};

	typedef std::vector<Tile> Tiles;

	class ParallelRenderer : public Renderer
	{
		private:
			Tiles
      worker (TaskDispatcher& _task_dispatcher, World& _world, Settings&
          _settings, Engine& _engine, SuperSampling* _super_sampling,
              volatile sig_atomic_t& interrupted);

      Image
      render_iteration(World& _world, Settings& _settings,
                       Engine& _engine, SuperSampling* _super_sampling,
                       volatile sig_atomic_t interrupted);

		public:
			Image
      render (World& _world, Settings& _settings, Engine& _engine,
          SuperSampling* _super_sampling);

	};
}

#endif

