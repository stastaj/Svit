#include "node/group/simple_group.h"

namespace Svit
{
	void 
	SimpleGroup::add (Node *_node)
	{
		nodes.push_back(_node);
	}
	
	bool SimpleGroup::intersect(const Ray& _ray, Intersection& _best) const
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

