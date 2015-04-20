#ifndef SVIT_MATH_NUMERIC
#define SVIT_MATH_NUMERIC

#include <cmath>
#include "geom/vector.h"


namespace Svit
{
  
	void
	decompose_float (float _float, int *_integral, float *_fractional, 
	    bool _abs = true);

	bool
	around_zero (float _float);

  Vector3
  sample_power_cos_hemisphere_w( const Vector2& _samples, const float  _power,
      float*_pdfw);

  float
  power_cos_hemisphere_pdf_w(const Vector3& _normal, const Vector3& _direction,
      const float  _power);

  Vector3
  sample_cos_hemisphere_w(const Vector2& _samples,float *_pdfW);

  float
  cos_hemisphere_pdf_w(const Vector3& _normal,const Vector3& _direction);

  float
  pdf_WtoA(const float _pdfW,const float _dist,const float _cos_there);

  float
  pdf_AtoW(const float _pdfA,const float _dist,const float _cos_there);
  
  Vector3 
  sample_uniform_sphere_w(const Vector2& samples,float* pdf);
  
  Vector3 sample_uniform_hemisphere_w(const Vector2& samples,float* pdf);

}

#endif

