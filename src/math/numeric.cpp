#include "math/numeric.h"


#include <limits>


namespace Svit
{
	void
	decompose_float (float _float, int *_integral, float *_fractional, 
	    bool _abs)
	{
		float fractional;	
		float integral;

		fractional = modff(_float, &integral);			
		
		if (_abs)
			fractional = fabs(fractional);

		*_integral = (int)floor(_float);
		*_fractional = fractional;
	}

	bool
	around_zero (float _float)
	{
		if (_float > 0.0 && _float < std::numeric_limits<float>::epsilon())
			return true;
		else
			return false;
	}

  //////////////////////////////////////////////////////////////////////////
  // Cosine lobe hemisphere sampling

  Vector3
  sample_power_cos_hemisphere_w(
      const Vector2& _samples,
      const float  _power,
      float        *_pdfw)
  {
      const float term1 = 2.f * PI_F * _samples.x;
      const float term2 = std::pow(_samples.y, 1.f / (_power + 1.f));
      const float term3 = std::sqrt(1.f - term2 * term2);

      if(_pdfw)
      {
          *_pdfw = (_power + 1.f) * std::pow(term2, _power) * (0.5f * INV_PI_F);
      }

      return Vector3(
          std::cos(term1) * term3,
          std::sin(term1) * term3,
          term2);
  }

  float power_cos_hemisphere_pdf_w(
      const Vector3& _normal,
      const Vector3& _direction,
      const float  _power)
  {
      const float cosTheta = std::max(0.f, (_normal % _direction));

      return (_power + 1.f) * std::pow(cosTheta, _power) * (INV_PI_F * 0.5f);
  }

  //////////////////////////////////////////////////////////////////////////
  /// Sample direction in the upper hemisphere with cosine-proportional pdf
  /** The returned PDF is with respect to solid angle measure */
  Vector3 sample_cos_hemisphere_w(
      const Vector2& _samples,
      float        *_pdfW)
  {
      const float term1 = 2.f * PI_F * _samples.x;
      const float term2 = std::sqrt(1.f - _samples.y);

      const Vector3 ret(
          std::cos(term1) * term2,
          std::sin(term1) * term2,
          std::sqrt(_samples.y));

      if(_pdfW)
      {
          *_pdfW = ret.z * INV_PI_F;
      }

      return ret;
  }

  float cos_hemisphere_pdf_w(
      const Vector3  &_normal,
      const Vector3  &_direction)
  {
      return std::max(0.f, (_normal % _direction)) * INV_PI_F;
  }

  //////////////////////////////////////////////////////////////////////////
  // Utilities for converting PDF between Area (a) and Solid angle (w)
  // WtoA = PdfW * cosine / distance_squared
  // AtoW = PdfA * distance_squared / cosine

  float pdf_WtoA(
      const float _pdfW,
      const float _dist,
      const float _cos_there)
  {
      return _pdfW * std::abs(_cos_there) / (_dist*_dist);
  }

  float pdf_AtoW(
      const float _pdfA,
      const float _dist,
      const float _cos_there)
  {
      return _pdfA * (_dist*_dist) / std::abs(_cos_there);
  }


}

