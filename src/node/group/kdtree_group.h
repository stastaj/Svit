#ifndef SVIT_KDTREE_GROUP
#define SVIT_KDTREE_GROUP

#include "node/group/group.h"
#include "node/aabb.h"
#include "geom/ray.h"
#include "node/kdtree.h"

#include <vector>

namespace Svit
{
	class KdTreeGroup : public Group
	{
		private:
      std::vector<Node*> nodes;
      std::vector<Node*> infinite_nodes;
      AABB bb;
      KdTreeNode* root;
      KdTree kdtree;
      
		public:
      
      KdTreeGroup():bb(Vector3(std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max()),
                       Vector3(-std::numeric_limits<float>::max(),
                               -std::numeric_limits<float>::max(),
                               -std::numeric_limits<float>::max()
                               )){}
      
			bool
      intersect (const Ray& _ray, Intersection& _best) override;

      AABB
      get_aabb() const override;

			void
			set_material (std::unique_ptr<Material> _material) override;

			void
			add (Node* _node) override;

			void
			build_kdtree () override ;
	};
}

#endif

