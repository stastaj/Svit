#ifndef SVIT_PATH_TRACING_ENGINE
#define SVIT_PATH_TRACING_ENGINE

#include "engine/engine.h"
#include "geom/vector.h"                // for Vector3

namespace Svit
{
  class Frame;
  class Material;
  class Ray;
  class SuperSampling;
  struct World;
  
  
	class PathTracing : public Engine
	{
		public:
      /**
       * @brief get_color Generates one path from the camera around the scene,
       * starting at _ray. At each point of the path direct illumination is 
       * evaluated -using multiple importance sampling (light and brdf sampling)
       * - and using next event estimation. The end of the path is determined
       * using russian rullete.   
       * @param _ray Generated ray from the camera.
       * @param _world virtual 3d world. 
       * @param _sampler Sampler for generating random numbers. 
       * @param _iteration Number of the pass - also number of the paths 
       * generated for one pixel.
       * @return RGB color - incoming radiance to the camera, from the direction
       * in the _ray parameter. 
       */
			Vector3
      get_color (const Ray& ray, const World& _world, SuperSampling* _sampler, 
                 const int _iteration, unsigned int& _ray_id) 
      const override;
    
    private:
      /**
       * @brief get_direct_illumination Estimates direct illumination for the 
       * given point in the scene. Combines light and brdf sampling estimators,
       * using multiple importance sampling and balance heuristic. 
       * @param frame Tangent coordinate frame for surface point surfpt. 
       * @param matID ID of the material in the surfpt. 
       * @param surfpt 3d coordinates of the point in the scene. 
       * @param wol Direction from which we came to this point surfpt, in local 
       * coordinates with respect to tangent frame.  
       * @param samples_brdf 2D random numbers used for brdf sampling.
       * @param samples_light 2D random numbers used for light sampling. 
       * @param _world Virtual scene description. 
       * @param inside_glass If we are inside glass.
       * @param _ray_id ID of the ray - also ray counter for one worker. 
       * @return Estimated direct illumination in given surface point. 
       */
      Vector3
      get_direct_illumination(const Frame& frame, const int matID,
                              const Point3& surfpt, const Vector3& wol,
                              Vector2& samples_brdf, Vector2& samples_light,
                              const World& _world, bool inside_glass, 
                              unsigned int& _ray_id) const;
            
      /**
       * @brief computeMISWeight Computes multiple importance sampling using 
       * balance heuristic. 
       * @param pdf1
       * @param pdf2
       * @return 
       */
      inline float 
      computeMISWeight(float pdf1,float pdf2) const {
        return pdf1/(pdf1+pdf2);
      }
      
	};
}

#endif

