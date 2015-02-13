#ifndef SVIT_MATH_NUMERIC
#define SVIT_MATH_NUMERIC

#include <cmath>
#include "geom/vector.h"


#define PI_F 3.14159265358979f
#define INV_PI_F 0.318309886f
#define PI_D 3.1415926535897932384626433832795028841971
#define INV_PI_D (1.0f / PI_D)

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

}

#endif

