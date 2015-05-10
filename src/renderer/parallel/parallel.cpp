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
  
  void
  ParallelRenderer::sleep_and_stop(int _seconds)
  {
    for(int i=0;i<_seconds;++i){
      sleep(1u);
      if(ParallelRenderer::interrupted)
        return;
    }
    ParallelRenderer::interrupted=true;
  }
  

	Image
  ParallelRenderer::worker (TaskDispatcher& _task_dispatcher, World& _world,
      Settings& _settings, Engine& _engine, SuperSampling* _super_sampling,
                     volatile sig_atomic_t& interrupted, int _worker_number)
	{
		Image result(_settings.resolution);
    while (!interrupted)
		{
			boost::optional<Task> optional_task = _task_dispatcher.get_task();
			if (!optional_task)
				break;
			Task task = optional_task.get(); // use iteration number??
			render_iteration(_world, _settings, _engine,_super_sampling, result, 
                       task, ray_ids[_worker_number]);
      ++result.iterations;
		}

    return result;
	}

	void 
  ParallelRenderer::render(World& _world, Settings& _settings, Engine&
      _engine, SuperSampling* _super_sampling, Image& _final_image)
	{
		TaskDispatcher task_dispatcher(_settings);
		std::vector<std::future<Image>> futures(0);
    std::vector<SuperSampling*> samplers;

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_sigaction = sig_handler;
    action.sa_flags     = SA_SIGINFO;
    sigaction(SIGINT, &action, NULL);   
    
    for (unsigned i = 0; i < _settings.max_thread_count; i++)
		{
      ray_ids.push_back(0u);
      samplers.push_back(_super_sampling->copy(i));
      SuperSampling* sampler=samplers[i];
			// TODO can this be done better? it must be possible
      futures.push_back(
            std::async(std::launch::async,
         [this, &task_dispatcher,&_world, &_settings, &_engine, sampler,i]
         ()
          {
           return worker(task_dispatcher, _world, _settings, _engine, sampler,
                         interrupted,i);
          }));
		}
    
    if(_settings.time>0){
      std::async(std::launch::async,
       [this, &_settings]()
          {
            sleep_and_stop(_settings.time);
          });
    }
    
    for (unsigned i = 0; i < futures.size(); i++)
		{
			futures[i].wait();
		}
    //interrupted=true;
    for(SuperSampling* s: samplers){
      delete s;
    }
    samplers.clear();

		for (unsigned i = 0; i < futures.size(); i++)
		{
      try{
        Image img= futures[i].get();
        _final_image.add_image(img);
        _final_image.iterations+=img.iterations;
        
      }
      catch (const std::future_error& e) {
        std::cout << "Caught a future_error with code \"" << e.code()
                         << "\"\nMessage: \"" << e.what() << "\"\n";
      }
		}
    _final_image.scale(1.0f/(float)_final_image.iterations);
	}

  void
  ParallelRenderer::render_iteration (World& _world, Settings& _settings,
      Engine& _engine, SuperSampling* _super_sampling, Image& _result, 
                                      const int _iteration, unsigned int& ray_id) const
  {
    for (int x = 0; x < _settings.resolution.x; x++)
    {
      for (int y = 0; y < _settings.resolution.y; y++)
      {
        Vector2 samples=_super_sampling->next_sample2();
        const Ray ray = _world.camera->get_ray(x, y, samples, ray_id);
        const Vector3 illum=_engine.get_color(ray, _world,_super_sampling, 
                                              _iteration, ray_id);
        _result.add_to_pixel(x, y, illum);
      }
    }
  }

}

