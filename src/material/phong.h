#ifndef SVIT_PHONG_MATERIAL
#define SVIT_PHONG_MATERIAL
             
#include "geom/point.h"                 // for Point3
#include "material/material.h"
#include "texture/texture.h"
#include "geom/vector.h"

#include <algorithm>       // for move
#include <memory>

namespace Svit
{
  class Frame;
  
  class Phong : public Material
	{
		private:
      std::unique_ptr<Texture> texture; // color - diffuse reflectance
      float exponent;
      Vector3 gloss_reflectance;
      /* For energy conservation must be satisfied
         that gloss_reflectance + texture < 1 for all three channels!!! */

		public:
      Phong (std::unique_ptr<Texture> _texture, float _exp, Vector3 _reflectance):
        texture(std::move(_texture)), exponent(_exp),
        gloss_reflectance(_reflectance)
      {}


      Vector3
      eval_brdf(const Point3& _point, const Vector3& _wil, const Vector3& _wol)
      const override;

      void
      sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& _type,
                  float& _ior)
      const override;

      float
      get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
              const Vector3& _wig)
      const override;
  };
}

#endif

