#ifndef SVIT_SIMPLE_GROUP
#define SVIT_SIMPLE_GROUP

#include "node/group/group.h"
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
      intersect (const Ray& _ray, Intersection& _best) override;

      AABB
      get_aabb() const override;


			void
			set_material (std::unique_ptr<Material> _material);

			void
			add (Node* _node);

			void
			finish () { }
	};
}

#endif

