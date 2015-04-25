#ifndef SVIT_ENGINE
#define SVIT_ENGINE

#include "geom/ray.h"
#include "world/world.h"
#include "node/node.h"
#include "node/solid/solid.h"
#include "supersampling/supersampling.h"

#include <list>

namespace Svit
{
  /**
   * @brief The Engine class represents an integrator of incoming radiance at 
   * the image plane in the case of physically based rendering. How the 
   * radiance is accumulated depends solely on the integrator - in monte 
   * carlo rendering this is accomplished by making random walks around the 
   * scene.
   */
	class Engine
	{
		public:
      /**
       * @brief get_color This method generates one light path from the camera,
       * starting at _ray parameter. 
       * @param _ray Generated ray from the camera.
       * @param _world virtual 3d world. 
       * @param _sampler Sampler for generating random numbers. 
       * @param _iteration Number of the pass - also number of the paths 
       * generated for one pixel.
       * @return RGB color - incoming radiance to the camera, from the direction
       * in the _ray parameter. 
       */
			virtual Vector3 
      get_color (const Ray& _ray, const World& _world, SuperSampling* _sampler,
                 const int _iteration, unsigned int& _ray_id) const = 0;
      
      /**
       * @brief is_occluded is a method for shadow rays - it tests visibility 
       * between two points in the scene. 
       * @param _world 3D scene representation.  
       * @param _point 3D coordinates of the first point - ray origin.
       * @param _distance distance to the second point to be tested in given 
       * direction.
       * @param _direction direction from the origin to the second point - 
       * normalized. 
       * @return If there something is in between the two points - if the ray is 
       * occluded.
       */
      bool
      is_occluded(const World& _world, const Vector3& _point, float _distance, 
                  const Vector3& _direction, unsigned int& _ray_id) const 
      {
        const Ray shadow_ray(_point+_direction*RAY_EPSILON, _direction,_ray_id,0);
        Intersection shadow_isect(_distance-2.0f*RAY_EPSILON);
        return _world.scene->intersect(shadow_ray, shadow_isect);
      }
	};
}

#endif

