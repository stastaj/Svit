#include "renderer/serial/serial.h"

#include <iostream>

namespace Svit
{
	Image
	SerialRenderer::render (World& _world, Settings& _settings, Engine& _engine,
      SuperSampling* _super_sampling)
	{
    int res_x = _settings.resolution.size.x;
    int res_y = _settings.resolution.size.y;

    Image result(_settings.resolution.size);


    for (int x = 0; x < res_x; x++)
      for (int y = 0; y < res_y; y++)
      {
        const Vector2 samples = _super_sampling->next_sample(x, y);
        const Vector2i pixel(x,y);
        Ray ray = _world.camera->get_ray(pixel, samples);
        result(x, y) = _engine.get_color(ray, _world);
      }



		return result;
	}
}

