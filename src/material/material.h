#ifndef SVIT_MATERIAL
#define SVIT_MATERIAL

#include "geom/vector.h"
#include "geom/point.h"
#include "math/frame.h"

namespace Svit
{
  enum reflection_type
  {
    diffuse,
    specular,
    glossy,
    refraction,
    reflection
  };

	/**
	*  @brief Class representing material BDRF(bidirectional reflectance function).
	* See http://cs.wikipedia.org/wiki/BRDF for definition.
	*/
	class Material
	{
		public:
     /**
     * @brief eval_brdf Evaluates the amount of reflected light from direction 
     * _wil to direction _wol.
     * 
		 * Physically based BRDF should satisfy following properties: 
		 * 			Positivity: eval_brdf(_point,_wil,_wol) >= 0, for each wil, wol, point.
		 * 			Helmolzs Reciprocity: eval_brdf(_point,_wil,_wol) == eval_brdf(_point,_wol,_wil).
		 *			Conserving energy: \forall \omega_{\text{r}},\, \int_\Omega f_{\text{r}}(\omega_{\text{i}},\, \omega_{\text{r}})\,\cos{\theta_{\text{i}}} d\omega_{\text{i}} \le 1
     * @param _point 3D Coordinates of the intersected point.
     * @param _wil (omega_incoming_local) Direction to the light in local coordinates with
     * respect to tangent coordinate frame (l stands for local). 
     * @param _wol  (omega_outgoing_local) Direction to the camera in local coordinates with
     * respect to tangent coordinate frame (l stands for local).
     * @return The amount of light reflected from direction 
     * _wil to direction _wol.
     */
    virtual Vector3
    eval_brdf(const Point3& _point, const Vector3& _wil, const Vector3& _wol)
    const = 0;
    
     /**
     * @brief sample_brdf Randomly sample next direction, where previous 
     * direction _wol, tangent frame, surface coordinates are given. Output the
     * pdf of sampled direction, brdf, reflection type, index of refraction,
     * and sampled direction in global coordinates.
     * 
     * @param _point 3D coordinates of surface point to sample the direction. 
     * @param _frame Tangent frame.
     * @param _pdf Output parameter, sampled probability density. 
     * @param _sampled_dir_global Output parameter, Sampled direction in global 
     * coordinates.
     * @param _brdf Output parameter, Brdf value of sampled direction. 
     * @param _wol Incoming direction in local coordinates. 
     * @param _samples Random numbers. 
     * @param _type Output parameter, sampled Reflection type.
     * @param inside_glass If ray is coming through glass object.
     */
    virtual void
    sample_brdf( const Point3& _point, const Frame& _frame,float* _pdf,
                Vector3& _sampled_dir_global, Vector3& _brdf,
                const Vector3& _wol,Vector2& _samples,reflection_type& _type, 
                 bool& inside_glass)
    const = 0;
    
      /**
     * @brief get_pdf Evaluates probability density function of sampling _wig 
     * direction as next direcion in random walk from camera -> for light incoming 
		 * and outgoing directions are swapped.
     * 
     * @param _point 3D surface point coordinates. 
     * @param _frame Tangent frame.
     * @param _wog Incoming direction in global coordinates.
     * @param _wig Outgoing direction in global coordinates.
     * @return The pdf of sampling _wog as next direction, where _wog is the 
     * incoming direction.
     */
    virtual float
    get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
            const Vector3& _wig)
    const = 0;

	};
}

#endif

