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
			Vector3
      get_color (const Ray& ray, const World& _world, SuperSampling* _sampler, 
                 const int _iteration) 
      const override;
    
    private:
      Vector3
      get_direct_illumination(const Frame& frame, const int matID,
                              const Point3& surfpt, const Vector3& wol,
                              Vector2& samples_brdf, Vector2& samples_light,
                              const World& _world, bool inside_glass) const;
            
      inline float 
      computeMISWeight(float pdf1,float pdf2) const {
        return pdf1/(pdf1+pdf2);
      }
      
	};
}

#endif

