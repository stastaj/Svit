#ifndef SVIT_KDTREE
#define SVIT_KDTREE


#include <vector>
#include "geom/vector.h"

namespace Svit
{
  enum Axis{X,Y,Z,Leaf};

  struct AABB{
    Vector3 min;
    Vector3 max;
  };

  class KdTreeNode
	{

    public:
      KdTreeNode(std::vector<Node*> objects){


      }

    private:
      KDTreeNode* left;
      KDTreeNode* right;
      Axis axis;
      float split;
      std::vector<Node*> primitives;
	};

  class KdTree{

    private:
      bool Terminate(int depth,std::vector<Node*> objects){
        return objects.size() < max_leaf_size || depth >= max_depth;
      }

      int max_leaf_size=2;
      int max_depth=20;
  };
}

#endif

