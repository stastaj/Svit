#ifndef SVIT_NODE
#define SVIT_NODE

#include <memory>
#include <list>
#include <iostream>

namespace Svit
{
  class AABB;
  class Material;
  class Ray;
  class Intersection;
  
  /**
   * @brief The Node class represents one scene node, without any geometry, 
   * light or material information, also without any transformation matrix.
   */
	class Node
	{
		public:
      /**
       * @brief intersect 
       * @param _ray
       * @param _intersection
       * @return 
       */
			virtual bool
      intersect (const Ray& _ray, Intersection& _intersection) const = 0;

      virtual AABB
      get_aabb() const =0;
	};
}

#endif

