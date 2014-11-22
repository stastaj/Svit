#include "node/solid/sphere2.h"

namespace Svit
{
	boost::optional<Intersection>
  Sphere2::intersect (const Ray& _ray, const float _best)
	{
    // http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection

    // we transform ray origin into object space (center == origin)
    const Vector3 transformedOrigin = _ray.origin - center;

    const float A = _ray.direction % _ray.direction;
    const float B = 2.0f * (_ray.direction % transformedOrigin);
    const float C = (transformedOrigin % transformedOrigin) - (radius * radius);

    // Must use doubles, because when B ~ sqrt(B*B - 4*A*C)
    // the resulting t is imprecise enough to get around ray epsilons
    const double disc = B*B - 4*A*C;

    if(disc < 0)
        return boost::optional<Intersection>();

    const double discSqrt = std::sqrt(disc);
    const double q = (B<0) ? ((-B - discSqrt) * 0.5f) : ((-B + discSqrt) * 0.5f);

    double t0 = q / A;
    double t1 = C / q;

    if(t0 > t1) std::swap(t0, t1);

    float resT;

    if(t0 > 0.01f && t0 < _best)
        resT = float(t0);
    else if(t1 > 0.01f && t1 < _best)
        resT = float(t1);
    else
        return boost::optional<Intersection>();

    Intersection intersection;
    intersection.t = resT;
    intersection.point = _ray(resT);
    intersection.node = this;

    boost::optional<Intersection> result(intersection);
    return result;
  }

	void
  Sphere2::complete_intersection (Intersection *_intersection)
	{
		_intersection->normal = ~(_intersection->point - center);
	}

	void
  Sphere2::dump (const char *_name, unsigned int _level)
	{
		std::cout << std::string(' ', _level*2) << _name << " = Sphere" <<
		    std::endl;
		center.dump("center", _level+1);
		std::cout << std::string(' ', (_level+1)*2)  << "radius = " << radius <<
		    std::endl;
	}
}

