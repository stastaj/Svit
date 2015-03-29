#ifndef SVIT_GLASS_MATERIAL
#define SVIT_GLASS_MATERIAL
             
#include "geom/point.h"                 // for Point3
#include "material/material.h"
#include "geom/vector.h"

#include <memory>

namespace Svit
{
  class Frame;
  
  class Glass : public Material
	{
		private:
      float ior;
      float ior_inv;
		public:
      Glass (float _ior=1.55f):ior(_ior),ior_inv(1.f/_ior)
      {}

      Vector3
      eval_brdf(const Point3& _point, const Vector3& _wil, const Vector3& _wol)
      const override;

      void
      sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& type,
                  float& ior_inv)
      const override;

      float
      get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
              const Vector3& _wig)
      const override;
      
      void
      ideal_specular(Vector3& _brdf, Vector3& _globDir, const Vector3& _wol,
                     const Frame& _frame) const;
  };
}

#endif

