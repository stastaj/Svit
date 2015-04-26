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
       * @brief intersect given _ray with the scene and store the intersection
       * to the _intersection object. 
       * @param _ray 
       * @param _intersection
       * @return If the scene was hit by the ray and the distance of the 
       * intersection was larger than _ray.t_min and lesser than 
       * _intersection.t. 
       */
			virtual bool
      intersect (const Ray& _ray, Intersection& _intersection) const = 0;
      
      /**
       * @brief get_aabb
       * @return Axis aligned bounding box of this scene node. 
       */
      virtual AABB
      get_aabb() const =0;
	};
}

#endif

