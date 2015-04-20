#include "node/group/kdtree_group.h"

namespace Svit
{
	void 
	KdTreeGroup::add (Node *_node)
	{
    AABB _bb=_node->get_aabb();
    for(int i=0;i<3;++i){
      if( std::isinf(_bb.min[i]) || std::isinf(_bb.max[i]) ){
        infinite_nodes.push_back(_node);
        return;
      }
    }
    bb.extend(_bb);
		nodes.push_back(_node);
	}
	
	bool KdTreeGroup::intersect(const Ray& _ray, Intersection& _best) const
	{
    float t_max=_best.t;
    bool intersected=kdtree.traverse(bb,_ray,_best);    
		if(! intersected)
      _best.t=t_max;
    for (auto node : infinite_nodes)
		{
      if(node->intersect(_ray, _best))
        intersected=true;
		}
		return intersected;
	}

  AABB
  KdTreeGroup::get_aabb() const {
    return bb;
  }
  
  void
  KdTreeGroup::finish(){
    kdtree.root=kdtree.build(nodes,bb.min,bb.max,0);
    assert( kdtree.check_tree(kdtree.root) );
  }
}

