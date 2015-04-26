#ifndef SVIT_PARALLEL_RENDERER
#define SVIT_PARALLEL_RENDERER

#include "renderer/settings.h"
#include "image/image.h"
#include "renderer/parallel/task_dispatcher.h"
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
      /**
       * @brief interrupted variable is used for early canceling the render,
       * each worker after every pass checks this variable - if it is true, 
       * the worker ends. It is also used when catching SIGINT signal to stop 
       * the render. 
       */
      static volatile sig_atomic_t interrupted;
      /**
       * @brief ray_ids Each ray has its own ray_id, which is unique for across
       * one worker - id is simple ray count. so every worker counts how many
       * rays where cast.
       */
      std::vector<unsigned int> ray_ids;
      
			Image
      worker (TaskDispatcher& _task_dispatcher, World& _world, Settings&
          _settings, Engine& _engine, SuperSampling* _super_sampling,
              volatile sig_atomic_t& interrupted, int _worker_number);

      void
      render_iteration(World& _world, Settings& _settings, Engine& _engine,
                       SuperSampling* _super_sampling, Image& _result, 
                       const int _iteration, unsigned int& ray_id) const;

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

