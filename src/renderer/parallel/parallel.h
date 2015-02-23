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

	class ParallelRenderer : public Renderer
	{
		private:
      static volatile sig_atomic_t interrupted;

			Image
      worker (TaskDispatcher& _task_dispatcher, World& _world, Settings&
          _settings, Engine& _engine, SuperSampling* _super_sampling,
              volatile sig_atomic_t& interrupted) const;

      Image
      render_iteration(World& _world, Settings& _settings,
                       Engine& _engine, SuperSampling* _super_sampling,
                       volatile sig_atomic_t interrupted) const;

      static void
      sig_handler(int sig, siginfo_t *siginfo, void *context);
      
      void
      sleep_and_stop(int _seconds);
      
		public:
			void
      render (World& _world, Settings& _settings, Engine& _engine,
          SuperSampling* _super_sampling, Image& _final_image) override;
	};
}

#endif

