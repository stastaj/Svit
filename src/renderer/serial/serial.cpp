#include "renderer/serial/serial.h"

#include <iostream>

namespace Svit
{
	void
	SerialRenderer::render (World& _world, Settings& _settings, Engine& _engine,
      SuperSampling* _super_sampling, Image& _final_image)
	{
    //Image result(_settings.resolution);
    for (int x = 0; x < _settings.resolution.x; x++)
      for (int y = 0; y < _settings.resolution.y; y++)
      {
        Vector2 samples=_super_sampling->next_sample();
        Ray ray = _world.camera->get_ray(x, y, samples);
        Vector3 illum=_engine.get_color(ray, _world, _super_sampling, 0);
        _final_image.add_to_pixel(x, y, illum);
      }



		//return result;
	}
}

