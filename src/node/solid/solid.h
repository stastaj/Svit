#ifndef SVIT_SOLID
#define SVIT_SOLID

#include "node/node.h"
#include "geom/ray.h"
#include "geom/intersection.h"
#include "material/material.h"

#include <list>
#include <memory>

namespace Svit
{
	class Solid : public Node
	{
		public:
		  std::unique_ptr<Material> material;

			void
			set_material (std::unique_ptr<Material> _material)
			{
				material = std::move(_material);	
			}

			std::list<Intersection>
			fail ()
			{
				return std::list<Intersection>();
			}

			virtual void
			complete_intersection (Intersection &_intersection, const Ray& _ray) 
      const = 0;

			virtual void
			dump (const char *_name, unsigned int _level = 0) = 0;
	};
}

#endif

