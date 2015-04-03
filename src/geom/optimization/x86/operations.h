#ifndef SVIT_GEOM_OPTIMIZATION_X86_OPERATIONS
#define SVIT_GEOM_OPTIMIZATION_X86_OPERATIONS

#include "geom/optimization/x86/general.h"
#include "geom/optimization/x86/vector.h"
#include <cmath>

namespace Svit
{
  const __m128 minus_one=_mm_set1_ps(-1.f);
  //const __m128 half=_mm_set1_ps(0.5f);
  //const __m128 three=_mm_set1_ps(3.f);
  
  
	inline Vector<float> 
	operator+(const Vector<float>& a, const Vector<float>& b)
	{
		return Vector<float>(_mm_add_ps(a.v, b.v));
	}

	inline Vector<float> 
	operator*(const Vector<float>& v, float s)
	{
		return Vector<float>(_mm_mul_ps(v.v, _mm_set1_ps(s)));
	}

  inline Vector<float>
  operator/(const Vector<float>& v, float s)
  {
    return Vector<float>(_mm_div_ps(v.v, _mm_set1_ps(s)));
  }
  
  inline Vector<float>
  operator/(const Vector<float>& v, const Vector<float>& w)
  {
    return Vector<float>(_mm_div_ps(v.v, w.v));
  }

	inline Vector<float> 
	operator*(float s, const Vector<float>& v)
	{
		return Vector<float>(_mm_mul_ps(v.v, _mm_set1_ps(s)));
	}

	inline Vector<float>
	operator-(const Vector<float>& a, const Vector<float>& b)
	{
		return Vector<float>(_mm_sub_ps(a.v, b.v));
	}

	inline Vector<float>
	operator&(const Vector<float>& a, const Vector<float>& b)
	{
		const __m128 t1 = _mm_shuffle_ps(a.v, a.v, 0xc9);
		const __m128 t2 = _mm_shuffle_ps(a.v, a.v, 0xd2);
		const __m128 t3 = _mm_shuffle_ps(b.v, b.v, 0xd2);
		const __m128 t4 = _mm_shuffle_ps(b.v, b.v, 0xc9);
		const __m128 t5 = _mm_mul_ps(t1, t3);
		const __m128 t6 = _mm_mul_ps(t2, t4);

		return Vector<float>(_mm_sub_ps(t5, t6));
	}

	inline float
	operator%(const Vector<float>& a, const Vector<float>& b)
	{
		#if defined(__SSE4_1__)
			const __m128 dp = _mm_dp_ps(a.v, b.v, 0xff);
		#elif defined(__SSE3__)
			const __m128 t1 = _mm_mul_ps(a.v, b.v);
			const __m128 t2 = _mm_hadd_ps(t1, t1);
			const __m128 dp = _mm_hadd_ps(t2, t2);
		#else/*
      const __m128 tmp=_mm_mul_ps(a.v,b.v);
      const __m128 t = _mm_add_ps(tmp, _mm_movehl_ps(tmp, tmp));
      const __m128 dp = _mm_add_ss(t, _mm_shuffle_ps(t, t, 1));
   */
    
      __m128 dp = _mm_mul_ps(a.v, b.v);
      __m128 v1 = _mm_shuffle_ps( dp, dp, _MM_SHUFFLE(2, 3, 0, 1));
      dp = _mm_add_ps(dp, v1);
      v1 = _mm_shuffle_ps(dp, dp, _MM_SHUFFLE(0, 1, 2, 3));
      dp = _mm_add_ps(dp, v1);
		#endif

    //float[4] f=dp;
		return _mm_cvtss_f32(dp); 
	}

  
  inline Vector<float>&
  operator+=(Vector<float>& v,const Vector<float>& _v2){
    v.v = _mm_add_ps(v.v, _v2.v);
    return v;
  }
  
  inline Vector<float>&
  operator*=(Vector<float>& v,const float f){
    v.v = _mm_mul_ps(v.v, _mm_set1_ps(f));
    return v;
  }
  
  inline Vector<float>&
  operator/=(Vector<float>& v,const float f){
    v.v = _mm_div_ps(v.v, _mm_set1_ps(f));
    return v;
  }
  
  inline Vector<float>&
  operator*=(Vector<float>& v,const Vector<float>& _v){
    v.v = _mm_mul_ps(v.v, _v.v);
    return v;
  }
  
  inline Vector<float>&
  operator/=(Vector<float>& v,const Vector<float>& _v){
    v.v = _mm_div_ps(v.v, _v.v);
    return v;
  }
  
  inline Vector<float>&
  operator-=(Vector<float>& v,const Vector<float>& _v){
    v.v = _mm_sub_ps(v.v, _v.v);
    return v;
  }
  
  
	inline Vector<float>
	operator!(const Vector<float>& v)
	{
    return Vector<float>(_mm_mul_ps(v.v, minus_one));
	}
  
	inline Vector<float>
	operator~(const Vector<float>& v)
	{
    __m128 v0 = _mm_mul_ps(v.v, v.v);
    __m128 v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(2, 3, 0, 1));
    v0 = _mm_add_ps(v0, v1);
    v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 1, 2, 3));
    v0 = _mm_add_ps(v0, v1);
    
    v1 = _mm_rsqrt_ps(v0);
    
    v0 = _mm_mul_ps( _mm_mul_ps( v0, v1 ), v1 ); 
    return _mm_mul_ps(v.v, _mm_mul_ps( _mm_mul_ps( half, v1 ), _mm_sub_ps( three, v0 ) )); 
    
    //return Vector<float> (_mm_div_ps(v.v, _mm_sqrt_ps(v0))); 
    
	}
  
  
}

#endif

