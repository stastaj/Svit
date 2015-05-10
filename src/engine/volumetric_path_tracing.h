#ifndef SVIT_VOLUMETRIC_PATH_TRACING_ENGINE
#define SVIT_VOLUMETRIC_PATH_TRACING_ENGINE

#include "engine/engine.h"
#include "geom/vector.h"                // for Vector3

namespace Svit
{
  class Frame;
  class Material;
  class Ray;
  class SuperSampling;
  struct World;
  
  
	class VolumetricPathTracing : public Engine
	{
    private:
      float sigma_s; // scattering coef
      float sigma_t; // extinction coeff
      float sigma_a; // attenuation coeff
      float albedo;
      
		public:
      VolumetricPathTracing(float _sigma_s, float _sigma_a):
        sigma_s(_sigma_s), sigma_a(_sigma_a), sigma_t(_sigma_a+_sigma_s) 
      {
        albedo=sigma_s/sigma_t;
      }
      
			Vector3
      get_color (const Ray& ray, const World& _world, SuperSampling* _sampler, 
                 const int _iteration, unsigned int& _ray_id) 
      const override;
    
    private:
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
      
      inline float 
      select_distance(float sample) const {
        return -std::log(1.f-sample)/sigma_t;
      }
      
	};
}

#endif

