#ifndef SVIT_KDTREE_GROUP
#define SVIT_KDTREE_GROUP

#include "node/group/group.h"
#include "geom/aabb.h"
#include "geom/ray.h"
#include "node/kdtree.h"

#include <vector>

namespace Svit
{
	class KdTreeGroup : public Group
	{
		private:
      std::vector<Solid*> nodes;
      std::vector<Solid*> infinite_nodes;
      AABB bb;
      KdTree kdtree;
      unsigned int solid_IDs;
		public:
      
      KdTreeGroup():bb(Vector3(std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max(),
                               std::numeric_limits<float>::max()),
                       Vector3(-std::numeric_limits<float>::max(),
                               -std::numeric_limits<float>::max(),
                               -std::numeric_limits<float>::max()
                               )),
                    solid_IDs(0)
      {}
      
			bool
      intersect (const Ray& _ray, Intersection& _best) const override;

      AABB
      get_aabb() const override;

			void
			add (Node* _node) override;

			void
			finish () override ;
	};
}

#endif

