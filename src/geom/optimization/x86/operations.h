#ifndef SVIT_GEOM_OPTIMIZATION_X86_OPERATIONS
#define SVIT_GEOM_OPTIMIZATION_X86_OPERATIONS

#include "geom/optimization/x86/general.h"
#include "geom/optimization/x86/vector.h"
#include <cmath>

namespace Svit
{
  const __m128 minus_one=_mm_set1_ps(-1.f);
  
  
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
		#else
      const __m128 tmp=_mm_mul_ps(a.v,b.v);
      const __m128 t = _mm_add_ps(tmp, _mm_movehl_ps(tmp, tmp));
      const __m128 dp = _mm_add_ss(t, _mm_shuffle_ps(t, t, 1));
    /*
			const __m128 t1 = _mm_mul_ps(a.v, b.v);
			const __m128 t2 = _mm_shuffle_ps(t1, t1, 0x93);
			const __m128 t3 = _mm_add_ps(t1, t2);
			const __m128 t4 = _mm_shuffle_ps(t3, t3, 0x4e);
			const __m128 dp = _mm_add_ps(t3, t4);*/
      
      //return a.x*b.x+a.y*b.y+a.z*b.z;
   
    /*
        __m128 dp = _mm_mul_ps(a.v, b.v);
    
        __m128 v1 = _mm_shuffle_ps( dp, dp, _MM_SHUFFLE(2, 3, 0, 1));
        dp = _mm_add_ps(dp, v1);
        v1 = _mm_shuffle_ps(dp, dp, _MM_SHUFFLE(0, 1, 2, 3));
        dp = _mm_add_ps(dp, v1);
    
        return _mm_cvtss_f32(dp);*/
    
		#endif

    //float[4] f=dp;
		return dp[0]; 
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

  inline 
  float inverse_sqrt_quake3(float x){
     float xhalf = 0.5f * x;
     int i = *(int*)&x;            // store floating-point bits in integer
     i = 0x5f3759df - (i >> 1);    // initial guess for Newton's method
     x = *(float*)&i;              // convert new bits into float
     x *= (1.5f - xhalf*x*x);     // One round of Newton's method
     x *= (1.5f - xhalf*x*x);  
     return x;
  }
  
	inline Vector<float>
	operator~(const Vector<float>& v)
	{
    /*__m128 half=_mm_mul_ps(_mm_set1_ps(0.5f),v.v);
    __m128 guess=_mm_mul_ps(v.v,_mm_rsqrt_ps(_mm_set1_ps(v % v)));
    
    Vector<float> result(_mm_mul_ps(guess,_mm_sub_ps(_mm_set1_ps(1.5f), _mm_mul_ps(half,_mm_mul_ps(guess,guess)))));                 
    return result;             */
    return Vector<float> (_mm_div_ps(v.v, _mm_sqrt_ps(_mm_set1_ps(v % v))));
    //return Vector<float> (_mm_mul_ps(v.v, _mm_set1_ps(inverse_sqrt_quake3(v % v))));
    //Vector<float> resultFast(_mm_mul_ps(v.v,_mm_rsqrt_ps(_mm_set1_ps(v % v))));
		//return result;
	}
  
  
}

#endif

