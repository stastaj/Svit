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
    refraction
  };
	class Material
	{
		public:
     /**
     * @brief eval_brdf
     * @param _point Coordinates of the intersected point.
     * @param _frame Coordinate frame representing tangent space in point
     * @param _point - z axis is the surface normal( in local coordinates
     * that is [0,0,1]).
     * @param _wil (omega_i) Direction to light in local coordinates with
     * respect to @param _frame (l stands for local).
     * @param _wol  (omega_o) Direction to camera in local coordinates with
     * respect to @param _frame (l stands for local).
     * @return
     */
    virtual Vector3
    eval_brdf(const Point3& _point, const Vector3& _wil, const Vector3& _wol)
    const = 0;

    virtual void
    sample_brdf( const Point3& _point, const Frame& _frame,float* _pdf,
                Vector3& _sampled_dir_global, Vector3& _brdf,
                const Vector3& _wol,Vector2& _samples,reflection_type type)
    const = 0;

    virtual float
    get_pdf(const Point3& _point,const Frame& _frame,const Vector3& _wog,
            const Vector3& _wig)
    const = 0;

	};
}

#endif

