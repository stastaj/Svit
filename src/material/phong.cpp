#include "material/phong.h"

using namespace Svit;

Vector3
Phong::eval_brdf(const Point3& _point, const Vector3& _wil,
                 const Vector3& _wol) const
{
  if( _wil.z <= 0 && _wol.z <= 0)
    return Vector3(0);

  Vector3 r(-_wil.x,-_wil.y,_wil.z);
  Vector3 glossyComponent  =
    gloss_reflectance*((exponent+2.0f)*0.5f*INV_PI_F)*(float)(pow((r % _wol),exponent));

  return texture->get_color(_point)*INV_PI_F + glossyComponent;
}

float
Phong::get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
        const Vector3& _wig) const
{
  float pd=(texture->get_color(_point)).max();
  float ps=gloss_reflectance.max();
  float sum=pd+ps;
  pd/=sum;
  ps/=sum;
  Vector3 refl=2*(_wog % _frame.mZ)*_frame.mZ;
  refl-=_wog;

  float spec=power_cos_hemisphere_pdf_w(refl,_wig,exponent);
  float diff=cos_hemisphere_pdf_w(_frame.mZ,_wig);

  return pd*diff+ps*spec;
}

void
Phong::sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol,Vector2& _samples,reflection_type type)
const
{
  Vector3 diffuseColor=texture->get_color(_point);
  float pd=diffuseColor.max();
  float ps=gloss_reflectance.max();
  float sum=pd+ps;
  pd/=sum;
  ps/=sum;

  if(_samples.x<=pd){
    //sample diffuse
    _samples.x*=(1.f/pd); //sample reuse

    //_samples.x=rand();

    Vector3 local=sample_cos_hemisphere_w(_samples,_pdf);
    _sampled_dir_global=~(_frame.to_world(local));
    _brdf=diffuseColor * INV_PI_F;
    *_pdf*=pd;
    type=diffuse;
  }else{
    //sample specular
    _samples.x=(_samples.x-pd)*(1.f/(ps)); // sample reuse

    Vector3 wog=_frame.to_world(_wol);
    Vector3 refl=2.0f*(wog % _frame.mZ)*_frame.mZ;
    refl=refl-wog;
    Frame local(refl);

    Vector3 local_dir=sample_power_cos_hemisphere_w(_samples,exponent,_pdf);
    _sampled_dir_global=~(local.to_world(local_dir));
    float cosThetaIn = _frame.mZ % _sampled_dir_global;
    if(cosThetaIn <= 0){
      _brdf=Vector3(0,0,0,0);
    }
    else{
      _brdf=gloss_reflectance * ((exponent+2.0f) * 0.5f * INV_PI_F *
                                   pow(local_dir.z,exponent));
    }
    *_pdf*=ps;
    type=specular;
  }
}
