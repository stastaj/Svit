#include "node/group/simple_group.h"

namespace Svit
{
	void 
	SimpleGroup::add (Node *_node)
	{
		nodes.push_back(_node);
	}

	void
	SimpleGroup::set_material (std::unique_ptr<Material> _material)
	{
		// TODO solve this problem:
		// unique_ptr does not allow to share the pointer we do not want to make
		// copy for each group member just imagine a huge model with 300k
		// triangles, sharing the same material with copying is insane. 
	}
	
	bool SimpleGroup::intersect(const Ray& _ray, Intersection& _best)
	{
    bool intersected=false;
		for (auto node : nodes)
		{
      if(node->intersect(_ray, _best))
        intersected=true;
        
		}
		return intersected;
	}

  AABB
  SimpleGroup::get_aabb() const {
    AABB bb=nodes[0]->get_aabb();
    for (Node* node : nodes)
    {
      AABB temp=node->get_aabb();
      bb.min.x=std::min(temp.min.x,bb.min.x);
      bb.min.y=std::min(temp.min.y,bb.min.y);
      bb.min.z=std::min(temp.min.z,bb.min.z);
      bb.max.x=std::max(temp.max.x,bb.max.x);
      bb.max.y=std::max(temp.max.y,bb.max.y);
      bb.max.z=std::max(temp.max.z,bb.max.z);
    }
    return bb;
  }
}

