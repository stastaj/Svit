#ifndef SVIT_X86_VECTOR
#define SVIT_X86_VECTOR

#include "geom/optimization/x86/general.h"

#include <math.h>
#include <assert.h>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace Svit
{
  const __m128 half=_mm_set1_ps(0.5f);
  const __m128 three=_mm_set1_ps(3.0f);
  
	template<>
	class alignas(16) Vector<float>
	{
		public:
			union
			{
				struct 
				{
					float x;
					float y;
					float z;
					float w;
				};
				__m128 v;
			};

      explicit inline Vector () :v (_mm_setr_ps(0.f,0.f,0.f,0.f))
			{ }
      
			explicit inline Vector (float _x , float _y = 0.0, 
                     float _z = 0.0, float _w = 0.0) :x(_x),y(_y),z(_z),w(_w)
			{ }

			inline Vector (const Vector& other):v(other.v)
			{ }

			inline Vector (__m128 other):v(other)
			{ }

      inline float
      max() const
      {
        return std::max(std::max(x,y),z);
      }
      
      inline float
      min() const
      {
        return std::min(std::min(x,y),z);
      }

      inline void
      normalize()
      {
        __m128 v0 = _mm_mul_ps(v, v);
        __m128 v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(2, 3, 0, 1));
        v0 = _mm_add_ps(v0, v1);
        v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 1, 2, 3));
        v0 = _mm_add_ps(v0, v1);
        
        v =_mm_div_ps(v, _mm_sqrt_ps(v0));
      }
      
      inline void
      normalize_fast() 
      {
        __m128 v0 = _mm_mul_ps(v, v);
        __m128 v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(2, 3, 0, 1));
        v0 = _mm_add_ps(v0, v1);
        v1 = _mm_shuffle_ps(v0, v0, _MM_SHUFFLE(0, 1, 2, 3));
        v0 = _mm_add_ps(v0, v1);
        
        //v =_mm_div_ps(v, _mm_sqrt_ps(v0));
        
        v1 = _mm_rsqrt_ps(v0);
        
        v0 = _mm_mul_ps( _mm_mul_ps( v0, v1 ), v1 ); 
        v =_mm_mul_ps(v, _mm_mul_ps( _mm_mul_ps( half, v1 ), _mm_sub_ps( three, v0 ) ));
      }
      
      inline float&
			operator[](int nth)
			{
				assert(nth >= 0 && nth <= 3);
				switch (nth)
				{
					case 0:
						return x;

					case 1:
						return y;

					case 2:
						return z;

					case 3:
						return w;
				}

				throw std::runtime_error("Bad index");
			}
      
      inline float
			operator[](int nth) const
			{
				assert(nth >= 0 && nth <= 3);
				switch (nth)
				{
					case 0:
						return x;
					case 1:
						return y;
					case 2:
						return z;
					case 3:
						return w;
				}
				throw std::runtime_error("Bad index");
			}
      
			inline void
			dump (const char *name, unsigned int level = 0)
			{
				std::string indentation(level*2, ' ');
				std::cout << std::setprecision(10) << std::fixed;
				std::cout << indentation << name << " = SSE Vector (" << x << ", " << 
						y << ", " << z << ", " << w << ")" << std::endl;
			}
    private:
      
	};
}

#endif

