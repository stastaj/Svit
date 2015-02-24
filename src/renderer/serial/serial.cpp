#include "renderer/serial/serial.h"

#include <iostream>

namespace Svit
{
	Image
	SerialRenderer::render (World& _world, Settings& _settings, Engine& _engine,
      SuperSampling* _super_sampling)
	{
    Image result(_settings.resolution);

    for (int x = 0; x < _settings.resolution.x; x++)
      for (int y = 0; y < _settings.resolution.y; y++)
      {
        Vector2 samples;
        _super_sampling->next_sample(samples);
        const Vector2i pixel(x,y);
        Ray ray = _world.camera->get_ray(pixel, samples);
        result.set_pixel(x, y,_engine.get_color(ray, _world));
      }



		return result;
	}
}

