#include "material/mirror.h"
#include "math/frame.h"                 // for Frame
#include "math/numeric.h"               // for cos_hemisphere_pdf_w, etc
#include "math/constants.h"

#include <assert.h>                     // for assert
#include <cmath>                        // for isnan, pow

using namespace Svit;

Vector3
Mirror::eval_brdf(const Point3& _point, const Vector3& _wil,
                 const Vector3& _wol) const
{
  (void)_point;
  (void)_wil;
  (void)_wol;
  return Vector3(0.f,0.f,0.f);
}

float
Mirror::get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
        const Vector3& _wig) const
{
  (void)_point;
  (void)_frame;
  (void)_wig;
  (void)_wog;
  return 0.f;
}

void
Mirror::ideal_specular(Vector3 &_brdf, Vector3 &_globDir, const Vector3 &_wol, 
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
Mirror::sample_brdf(const Point3& _point, const Frame& _frame, float* _pdf,
                  Vector3& _sampled_dir_global, Vector3& _brdf,
                  const Vector3& _wol, Vector2& _samples, reflection_type& _type,
                   bool& inside_glass) const
{
  (void)_point;
  *_pdf = 1.f;
  
  ideal_specular(_brdf,_sampled_dir_global,_wol,_frame);
	_type=reflection;
}
