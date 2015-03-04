#ifndef SVIT_RAY_TRACING_ENGINE
#define SVIT_RAY_TRACING_ENGINE

#include "engine/engine.h"
#include "light/light.h"
#include "math/numeric.h"
#include "math/frame.h"
#include "geom/ray.h"
#include "geom/intersection.h"
#include "material/material.h"

namespace Svit
{
	class PathTracing : public Engine
	{
		public:
			Vector3
      get_color (const Ray& ray, const World& _world, SuperSampling* _sampler) 
      const override;
	};
}

#endif

