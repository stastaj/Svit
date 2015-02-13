#ifndef SVIT_ENGINE
#define SVIT_ENGINE

#include "geom/ray.h"
#include "world/world.h"
#include "node/node.h"
#include "node/solid/solid.h"

#include <list>
#include <boost/optional.hpp>

namespace Svit
{
	class Engine
	{
		public:
			virtual Vector3 
      get_color (const Ray& _ray, const World& _world) const = 0;
      
	};
}

#endif

