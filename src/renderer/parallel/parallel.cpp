#include "renderer/parallel/parallel.h"
#include "renderer/serial/serial.h"

#include <thread>
#include <future>
#include <boost/optional.hpp>
#include <iostream>



namespace Svit
{  
  volatile sig_atomic_t ParallelRenderer::interrupted=false;
  
  void
  ParallelRenderer::sig_handler(int sig, siginfo_t *siginfo, void *context)
  {
    if(interrupted)
      std::exit(1);
    interrupted=true;
  }

	Tiles
  ParallelRenderer::worker (TaskDispatcher& _task_dispatcher, World& _world,
      Settings& _settings, Engine& _engine, SuperSampling* _super_sampling,
                     volatile sig_atomic_t& interrupted)
	{
		Tiles result;
    while (!interrupted)
		{
			boost::optional<Task> optional_task = _task_dispatcher.get_task();
			if (!optional_task)
				break;
			Task task = optional_task.get();

			Tile tile;
			tile.task = task;
			tile.image = render_iteration(_world, _settings, _engine,
                                    _super_sampling,interrupted);
      
			result.push_back(tile);
		}

    return result;
	}

	Image
  ParallelRenderer::render (World& _world, Settings& _settings, Engine&
      _engine, SuperSampling* _super_sampling)
	{
		TaskDispatcher task_dispatcher(_settings);
		std::vector<std::future<Tiles>> futures(0);
    std::vector<SuperSampling*> samplers;

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_sigaction = sig_handler;
    action.sa_flags     = SA_SIGINFO;
    sigaction(SIGINT, &action, NULL);

    for (unsigned i = 0; i < _settings.max_thread_count; i++)
		{
      samplers.push_back(_super_sampling->copy(i));
      SuperSampling* sampler=samplers[i];
			// TODO can this be done better? it must be possible
      futures.push_back(
            std::async(std::launch::async,
         [this, &task_dispatcher,&_world, &_settings, &_engine, sampler]
         ()
          {
           return worker(task_dispatcher, _world, _settings, _engine, sampler,
                         interrupted);
          }));
		}

		for (unsigned i = 0; i < futures.size(); i++)
		{
			futures[i].wait();
		}
    for(SuperSampling* s: samplers){
      delete s;
    }
    samplers.clear();

    int iterations=0;
    Image final_image(_settings.resolution);
		for (unsigned i = 0; i < futures.size(); i++)
		{
      try{
        Tiles tiles = futures[i].get();

        for(unsigned k = 0; k< tiles.size(); k++){
          final_image.add_image(tiles[k].image);
          iterations++;
        }
      }
      catch (std::exception&) {
      }
		}
    final_image.scale(1.0f/(float)iterations);
    final_image.iterations=iterations;
		return final_image;
	}

  Image
  ParallelRenderer::render_iteration (World& _world, Settings& _settings,
      Engine& _engine, SuperSampling* _super_sampling,
      volatile sig_atomic_t interrupted)
  {
    int res_x = _settings.resolution.x;
    int res_y = _settings.resolution.y;

    Image result(_settings.resolution);


    for (int x = 0; x < res_x; x++)
    {
      if(interrupted)
        return Image(_settings.resolution);
      for (int y = 0; y < res_y; y++)
      {
        const Vector2 samples = _super_sampling->next_sample(x, y);
        const Vector2i pixel(x,y);
        const Ray ray = _world.camera->get_ray(pixel, samples);
        result.set_pixel(x, y, _engine.get_color(ray, _world));
      }
    }


    return result;
  }

}

