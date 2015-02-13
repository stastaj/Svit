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
	template<>
	class Vector<float>
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

			inline Vector (float _x = 0.0, float _y = 0.0, 
                     float _z = 0.0, float _w = 0.0) :x(_x),y(_y),z(_z),w(_w)
			{ 
				//v = _mm_setr_ps(_x, _y, _z, _w);
			}

			inline Vector (const Vector& other)
			{ 
				v = other.v;
			}

			inline Vector (__m128 other)
			{ 
				v = other;
			}

      float
      max() const
      {
        return std::max(std::max(x,y),std::max(z,w));
      }

      void
      normalize()
      {
        const __m128 tmp=_mm_mul_ps(v,v);
        const __m128 t = _mm_add_ps(tmp, _mm_movehl_ps(tmp, tmp));
        const __m128 sum = _mm_add_ss(t, _mm_shuffle_ps(t, t, 1));
        
        v *= 1.f / (float)sqrt(sum[0]);
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

			inline void
			dump (const char *name, unsigned int level = 0)
			{
				std::string indentation(level*2, ' ');
				std::cout << std::setprecision(10) << std::fixed;
				std::cout << indentation << name << " = SSE Vector (" << x << ", " << 
						y << ", " << z << ", " << w << ")" << std::endl;
			}
	};
}

#endif

