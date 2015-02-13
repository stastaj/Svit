#ifndef SVIT_NODE
#define SVIT_NODE

//#include "geom/ray.h"
//#include "geom/intersection.h"
//#include "material/material.h"
#include "node/aabb.h"

#include <memory>
#include <list>
#include <iostream>

namespace Svit
{
  class Material;
  class Ray;
  class Intersection;
  
	class Node
	{
		public:
			virtual void
			set_material (std::unique_ptr<Material> _material) = 0;

			virtual bool
      intersect (const Ray& _ray, Intersection& _intersection) = 0;

      virtual AABB
      get_aabb() const =0;
	};
}

#endif

