#ifndef SVIT_SIMPLE_GROUP
#define SVIT_SIMPLE_GROUP

#include "node/group/group.h"
#include "node/aabb.h"
#include "geom/ray.h"

#include <vector>

namespace Svit
{
	class SimpleGroup : public Group
	{
		private:
      std::vector<Node*> nodes;

		public:
			bool
      intersect (const Ray& _ray, Intersection& _best) const override;

      AABB
      get_aabb() const override;

			void
			add (Node* _node) override;

			void
      build_kdtree () override { }
	};
}

#endif

