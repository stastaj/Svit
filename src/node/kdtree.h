#ifndef SVIT_KDTREE
#define SVIT_KDTREE

#include "geom/vector.h"
#include "node/aabb.h"
#include "boost/thread.hpp"

#include <vector>
#include <limits>


namespace Svit
{
  enum Axis{X,Y,Z,Leaf};
  
  class KdTreeNode{
    public:
      KdTreeNode():axis(Leaf) {}
      
      ~KdTreeNode(){
        if(axis!=Leaf){
          delete left;
          delete right;
        }
        else
          primitives.clear();
      }
      
      KdTreeNode* left;
      KdTreeNode* right;
      Axis axis;
      float split;
      std::vector<Node*> primitives;
  };
  
  struct StackEntry { 
    public:
      KdTreeNode* node; // far child pointer
      Point3 coord; // coord of entry/exit point 
      float t; // distance
      int prev;
  };
  
  class KdTree
  {
    public:
      KdTree() {}
      
      bool
      traverse(KdTreeNode* root, const AABB& bb, const Ray& _ray, Intersection& _intersection);
      
      KdTreeNode*
      build(std::vector<Node*>& _primitives, Vector3 _min, Vector3 _max,
            int _depth); 
      
    private:
      void
      split_primitives(std::vector<Node*> _primitives,float _split,Axis _axis,
                       std::vector<Node*> _left, std::vector<Node*> _right);
      
      bool
      terminate(int depth,std::vector<Node*>& _objects);
      
      void
      find_plane(Vector3& _min,Vector3& _max,float& _split,Axis& _axis);
      
      
      thread_local static std::vector<StackEntry> stack;
  };
  
}

#endif

