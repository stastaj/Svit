#ifndef SVIT_MATH_CONSTANTS
#define SVIT_MATH_CONSTANTS

#include <limits>

namespace Svit
{
  
  const float RAY_EPSILON=0.001f;
  const float EPS_DIVISION=0.000001; //std::numeric_limits<float>::epsilon(); 
  
  const float PI_F=3.1415926535897932384626433832795028841971f;
  const float INV_PI_F=(1.0f / PI_F);
  
  const double PI_D=3.1415926535897932384626433832795028841971;
  const double INV_PI_D=(1.0 / PI_D);

}

#endif

