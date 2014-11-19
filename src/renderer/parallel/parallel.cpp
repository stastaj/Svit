#include "renderer/parallel/parallel.h"
#include "renderer/serial/serial.h"

#include <thread>
#include <future>
#include <vector>
#include <utility>
#include <boost/optional.hpp>
#include <iostream>

namespace Svit
{
	Tiles
	ParallelRenderer::worker (TaskDispatcher& _task_dispatcher, World& _world, 
      Settings& _settings, Engine& _engine, SuperSampling* _super_sampling)
	{
		Tiles result;
		SerialRenderer serial_renderer;

		while (1)
		{

			boost::optional<Task> optional_task = _task_dispatcher.get_task();
			if (!optional_task)
				break;

			Task task = optional_task.get();

      Image rendered_image = serial_renderer.render(_world, _settings, _engine,
			    _super_sampling);

			Tile tile;
			tile.task = task;
			tile.image = rendered_image;
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

		for (unsigned i = 0; i < _settings.max_thread_count; i++)
		{
      samplers.push_back(_super_sampling->copy(i));
      SuperSampling* sampler=samplers[i];
			// TODO can this be done better? it must be possible
      futures.push_back(
            std::async(std::launch::async,
         [this, &task_dispatcher,&_world, &_settings, &_engine, sampler ]
         ()
          {
           return worker(task_dispatcher, _world, _settings, _engine, sampler);
          }));
		}
		for (unsigned i = 0; i < futures.size(); i++)
		{
			futures[i].wait();
		}
    samplers.clear();

    Image final_image(_settings.resolution.size);
		for (unsigned i = 0; i < futures.size(); i++)
		{
			Tiles tiles = futures[i].get();

      for(unsigned k = 0; k< tiles.size(); k++){
        final_image.add_image(tiles[k].image);
      }
		}
    final_image.scale(1.0f/(float)_settings.iterations);

		return final_image;
	}
}

