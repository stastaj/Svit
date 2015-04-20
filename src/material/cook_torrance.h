#ifndef SVIT_COOK_TORRANCE_MATERIAL
#define SVIT_COOK_TORRANCE_MATERIAL
             
#include "geom/point.h"                 // for Point3
#include "material/material.h"
#include "geom/vector.h"
#include "texture/texture.h"

#include <memory>

namespace Svit
{
  class Frame;
  
  class CookTorrance : public Material
	{
		private:
			float ka = 1.f;
			float ks = 0.8f;
			float kd = 0.2f;
			float ior = 1.3f;
			float roughness = 0.2f;
			Vector3 specular;//( 1.f, 1.f, 1.f );
			std::unique_ptr<Texture> texture; // color - diffuse reflectance //( 0.6f, 0.6f, 0.6f );
			float gauss_constant  = 100.f;
		public:
      CookTorrance ( std::unique_ptr<Texture> _texture, 
                     Vector3 _specular=Vector3(1.f,1.f,1.f),
                    float _roughness=0.2f, float _ior=1.3f, float _kd=0.2f, 
                    float _ks=0.8f, float _ka=1.f, float _gauss=100.f )
        : roughness(_roughness), ior(_ior), kd(_kd), ks(_ks), ka(_ka), 
          gauss_constant(_gauss), texture(std::move(_texture)), specular(_specular)
      {}

      Vector3
      eval_brdf(const Point3& _point, const Vector3& _wil, const Vector3& _wol)
      const override;

      void
      sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& type,
                  bool& inside_glass)
      const override;

      float
      get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
              const Vector3& _wig)
      const override;   
      
      float 
      schlicks_approx(Vector3& half, const Vector3& to_camera) const;
      
      float 
      Distribution(float NoH) const;
      
      float 
      chiGGX(float v) const;
      
      float 
      GGX_PartialGeometryTerm(const Vector3& _wol, const float& alpha, float VdotH) 
      const;
  };
}

#endif

