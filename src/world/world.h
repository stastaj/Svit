#ifndef SVIT_WORLD
#define SVIT_WORLD

#include "camera/camera.h"
#include "node/group/group.h"
#include "light/light.h"

#include <vector>
#include <memory>

namespace Svit
{
	struct World
	{
		Group *scene;
		Camera *camera;
		std::vector<std::unique_ptr<Light>> lights;
    std::vector<std::unique_ptr<Material>> materials;
    
		void
		add_light (std::unique_ptr<Light> _light)
		{
			lights.push_back(std::move(_light));	
		}
    
    int
		add_material (std::unique_ptr<Material> _material)
		{
			materials.push_back(std::move(_material));	
      return materials.size()-1;
		}
	};
}

#endif

