#ifndef SVIT_LIGHT
#define SVIT_LIGHT

#include "geom/vector.h"
#include "geom/point.h"
#include "math/frame.h"
 
namespace Svit
{
  enum LightType { Point, Area, Background };

  /**
   * @brief The Light class represents an emitor in our virtual scenes.
   */
	class Light 
	{
    protected:
      Light(const LightType _type):type(_type){}
		
    public:      
      const LightType type;
      
      /**
       * @brief sample_light Samples the light source and sets _wig, _light_dist
       * and _pdf variables. 
       * @param _surface_point 3d coordinates of last intersection with scene.
       * @param _frame coordinate tangent frame of last intersection with scene.
       * @param _samples 2d vector with 2 random numbers.
       * @param _wig output direction to the sampled point on the light source,
       * normalized, and in global coordinates.
       * @param _light_dist squared distance to the sampled point on the light 
       * source.
       * @param _pdf probability density function of sampling this point - with
       * respect to area probability - not solid angle from intersected point. 
       * @return 
       */
			virtual Vector3 
      sample_light ( const Point3& _surface_point, const Frame& _frame, 
                     const Vector2& _samples, Vector3& _wig, float& _light_dist,
                     float& _pdf) const = 0;
      
      /**
       * @brief get_radiance returns the amount of light emitted from light in
       * opposite direction to the omega.
       * 
       * @param wig Direction to the light source in global coordinates. 
       * @return the amount of light emitted from light in
       * opposite direction to the omega.
       */
			virtual Vector3 
      get_radiance ( const Vector3& wig ) const = 0;
      
      /**
       * @brief get_pdf
       * @param _wig direction to the light source in global coordinates. 
       * @param _light_dist_sqr squared distance to the last intersected point.
       * @return probability of sampling omega direction from this light source
       * as solid angle probability.
       */
      virtual float 
      get_pdf(const Vector3& _wig,float& _light_dist_sqr) const = 0;
	};
}

#endif

