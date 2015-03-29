#include "material/phong.h"
#include "math/constants.h"
#include "math/frame.h"                 // for Frame
#include "math/numeric.h"               // for cos_hemisphere_pdf_w, etc

#include <assert.h>                     // for assert
#include <cmath>                        // for isnan, pow

using namespace Svit;

Vector3
Phong::eval_brdf(const Point3& _point, const Vector3& _wil,
                 const Vector3& _wol) const
{
  if( _wil.z <= 0 && _wol.z <= 0)
    return Vector3(0.0f,0.0f,0.0f);

  Vector3 r(-_wil.x,-_wil.y,_wil.z);
  Vector3 glossyComponent  =
    gloss_reflectance*((exponent+2.0f)*0.5f*INV_PI_F)*(float)(pow((r % _wol),exponent));
  Vector3 diffuse=texture->get_color(_point)*INV_PI_F;
  
  assert(! std::isnan(diffuse.x));
  assert(! std::isnan(glossyComponent.x));
  
  return diffuse + glossyComponent;
}

float
Phong::get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
        const Vector3& _wig) const
{
  float pd=(texture->get_color(_point)).max();
  float ps=gloss_reflectance.max();
  float sum_inv=1.f/(pd+ps);
  pd*=sum_inv;
  ps*=sum_inv;
  Vector3 refl=2*(_wog % _frame.mZ)*_frame.mZ;
  refl-=_wog;

  float spec=power_cos_hemisphere_pdf_w(refl,_wig, exponent);
  float diff=cos_hemisphere_pdf_w(_frame.mZ,_wig);

  return pd*diff+ps*spec;
}

void
Phong::sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& _type,
                   float& _ior) const
{
  (void)_ior;
  Vector3 diffuseColor=texture->get_color(_point);
  float pd=diffuseColor.max();
  float ps=gloss_reflectance.max();
  float sum_inv=1.f/(pd+ps);
  pd*=sum_inv;
  ps*=sum_inv;

  if(_samples.x<=pd){
    //sample diffuse
    _samples.x /= pd; //sample reuse

    assert(_samples.x >= 0.f && _samples.x <= 1.f);
    
    //_samples.x=rand();

    Vector3 local=sample_cos_hemisphere_w(_samples,_pdf);
    _sampled_dir_global=_frame.to_world(local);
    _sampled_dir_global.normalize_fast();
    
    assert( _frame.mZ % _sampled_dir_global > 0 );
    
    _brdf=diffuseColor * INV_PI_F;
    *_pdf*=pd;
    _type=diffuse;
  }else{
    //sample specular
    _samples.x=(_samples.x-pd)*(1.f/(ps)); // sample reuse

    Vector3 wog=_frame.to_world(_wol);
    Vector3 refl=2.0f*(wog % _frame.mZ)*_frame.mZ;
    refl=refl-wog;
    Frame local(refl);

    Vector3 local_dir=sample_power_cos_hemisphere_w(_samples,exponent,_pdf);
    _sampled_dir_global=local.to_world(local_dir);
    _sampled_dir_global.normalize();
    float cosThetaIn = _frame.mZ % _sampled_dir_global;
    if(cosThetaIn <= 0){
      _brdf=Vector3(0,0,0,0);
    }
    else{
      _brdf=gloss_reflectance * ((exponent+2.0f) * 0.5f * INV_PI_F *
                                   pow(local_dir.z,exponent));
    }
    *_pdf*=ps;
    _type=specular;
  }
}
