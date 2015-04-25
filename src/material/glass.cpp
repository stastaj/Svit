#include "material/glass.h"
#include "math/frame.h"                 // for Frame
#include "math/numeric.h"               // for cos_hemisphere_pdf_w, etc
#include "math/constants.h"

#include <assert.h>                     // for assert
#include <cmath>                        // for isnan, pow

using namespace Svit;

Vector3
Glass::eval_brdf(const Point3& _point, const Vector3& _wil,
                 const Vector3& _wol) const
{
  (void)_point;
  (void)_wil;
  (void)_wol;
  return Vector3(0.f,0.f,0.f);
}

float
Glass::get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
        const Vector3& _wig) const
{
  (void)_point;
  (void)_frame;
  (void)_wig;
  (void)_wog;
  return 0.f;
}

void
Glass::ideal_specular(Vector3 &_brdf, Vector3 &_globDir, const Vector3 &_wol, 
                      const Frame& _frame) const
{
   Vector3 wog=_frame.to_world(_wol);
   float cosTheta=std::abs(wog % _frame.normal);
   Vector3 refl=2.f*(cosTheta)*_frame.normal;
   refl-=wog;
   //assert( abs(refl % refl - 1) < 0.001f );
   _globDir=~(refl);
   if(cosTheta<=EPS_DIVISION)
     _brdf=Vector3(0.f,0.f,0.f);
   else{
     _brdf= Vector3(1.f,1.f,1.f);
     _brdf/=cosTheta;
   }               
  
}

void
Glass::sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& _type,
                   bool& inside_glass) const
{
  (void)_point;
  *_pdf = 1.f;
  float n12,n1,n2;
  if(inside_glass){ // ray leaving glass
    n1 = ior;
    n2 = 1.0f;
  }
  else{ // ray entering glass
    n1 = 1.0f;
    n2 = ior;
  }
  n12 = n1/n2;
  float cosTheta_i = std::abs(_wol.z);
  assert(cosTheta_i >= 0);
  float f =  1.f - n12*n12 * (1.f - cosTheta_i*cosTheta_i );
  if (f <= 0){ // total reflection
    _type=reflection;
    ideal_specular(_brdf,_sampled_dir_global,_wol,_frame);
    return;
  }
  float cosTheta_t = sqrt(f);
  float rs = std::pow((n1*cosTheta_i-n2*cosTheta_t)/(n1*cosTheta_i+n2*cosTheta_t),2);
  float rp = std::pow((n1*cosTheta_t-n2*cosTheta_i)/(n1*cosTheta_t+n2*cosTheta_i),2);
  float r = (rs + rp)*0.5f;
  if (_samples.x <= r) { // odrazeny paprsek
    ideal_specular(_brdf,_sampled_dir_global,_wol,_frame);
    _type=reflection;
    return;
  }  
  // zalomeny paprsek
  inside_glass=!inside_glass;
  Vector3 wol2(_wol);
  wol2.z=std::abs(_wol.z);
  Vector3 light=_frame.to_world(!wol2);	
  _sampled_dir_global = (n12 * light) + (n12*cosTheta_i - cosTheta_t)*(_frame.normal);
  _sampled_dir_global.normalize();
  if(cosTheta_t <= EPS_DIVISION){
    _brdf=Vector3(0,0,0);
  }else{
    _brdf  = Vector3(1.0f,1.0f,1.0f);
    _brdf /= cosTheta_t;
    
  }
  _type=refraction;
}
