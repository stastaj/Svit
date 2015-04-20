#include "material/cook_torrance.h"
#include "math/frame.h"                 // for Frame
#include "math/numeric.h"               // for cos_hemisphere_pdf_w, etc
#include "math/constants.h"

#include <assert.h>                     // for assert
#include <cmath>                        // for isnan, pow


using namespace Svit;
using std::min;

Vector3
CookTorrance::eval_brdf(const Point3& _point, const Vector3& _wil,
                 const Vector3& _wol) const
{
  float NdotV = _wol.z; // cosinus of normal and direction to camera
  Vector3 H = ~(_wol+_wil);
  
  float NdotH = H.z;
  float NdotL = _wil.z;
  float VdotH = _wol % H;
  
  float D = Distribution(NdotH);
  float G = min(1.f, min((2*NdotH*NdotV/VdotH), (2*NdotH*NdotL/VdotH)));
  
  float cook=schlicks_approx(H,_wol) * D *G / ( 4.f * NdotL * NdotV );
  
  // (Fresnel / Pi)*D()*G() / ( NdotL * NdotV )
  return texture->get_color(_point)*INV_PI_F + specular*cook;
}

float
CookTorrance::get_pdf(const Point3& _point, const Frame& _frame,
                      const Vector3& _wog, const Vector3& _wig) const
{
  (void)_point;
  return INV_PI_F * 0.5f;
}

void
CookTorrance::sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& _type,
                   bool& inside_glass) const
{
  (void)inside_glass;
  
  Vector3 local_dir=sample_uniform_hemisphere_w(_samples,_pdf);
  _sampled_dir_global= ~ _frame.to_world(local_dir);
  
  _brdf=eval_brdf(_point,local_dir,_wol);
  
	_type=reflection_type::diffuse;
}

float 
CookTorrance::schlicks_approx(Vector3& half, const Vector3& to_camera) const
{
  float R0=((1.f-ior)*(1.f-ior))/((1.f+ior)*(1.f+ior));  
  return R0 + (1.f-R0)*std::pow((1.f- (half%to_camera)),5.f);
}

float 
CookTorrance::Distribution(float NdotH) const
{
    float alpha = std::acos(NdotH);
    return gauss_constant*exp(-(alpha*alpha)/(roughness*roughness));
}

float 
CookTorrance::chiGGX(float v) const
{
    return v > 0 ? 1 : 0;
}

float 
CookTorrance::GGX_PartialGeometryTerm(const Vector3& _wol, const float& alpha, float VdotH) 
const
{
    float VoH2 =  VdotH ;
    float chi = chiGGX( VoH2 / (_wol.z) );
    VoH2 = VoH2 * VoH2;
    float tan2 = ( 1.f - VoH2 ) / VoH2;
    return (chi * 2.f) / ( 1.f + std::sqrt( 1.f + alpha * alpha * tan2 ) );
}
