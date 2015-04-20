#ifndef SVIT_NODE
#define SVIT_NODE

//#include "geom/ray.h"
//#include "geom/intersection.h"
//#include "material/material.h"
//#include "geom/aabb.h"

#include <memory>
#include <list>
#include <iostream>

namespace Svit
{
  class AABB;
  class Material;
  class Ray;
  class Intersection;
  
	class Node
	{
		public:

			virtual bool
      intersect (const Ray& _ray, Intersection& _intersection) const = 0;

      virtual AABB
      get_aabb() const =0;
	};
}

#endif

