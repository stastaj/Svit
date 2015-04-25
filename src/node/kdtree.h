#ifndef SVIT_KDTREE
#define SVIT_KDTREE

#include "node/axis.h"
#include "geom/vector.h"
#include "geom/aabb.h"
#include "boost/thread.hpp"

#include <vector>
#include <limits>


namespace Svit
{  
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
  
  
  class KdTree{
    public:
      KdTreeNode* root;
      
      bool
      traverse(const AABB& bb, const Ray& _ray, 
               Intersection& _intersection) const;
      
      KdTreeNode*
      build(std::vector<Node*>& _primitives, Vector3 _min, Vector3 _max,
            int _depth); 
      
      bool
      check_tree(KdTreeNode* node){
        switch(node->axis){
          case X:
          case Y:
          case Z:
            if(node->left==nullptr || node->right==nullptr)
              return false;
            return check_tree(node->left) && check_tree(node->right);
            break;
          case Leaf:
            if(node->right != nullptr || node->left != nullptr){
              return false;
            }
            //if(node->primitives.empty())
            for(Node* n: node->primitives){
              if(n==nullptr)
                return false;
            }
            break;
          default:
            return false;
        }
        return true;
      }
    private:
      void
      split_primitives(std::vector<Node*>& _primitives, float _split, Axis _axis,
                       std::vector<Node*>& _left, std::vector<Node*>& _right);
      
      bool
      terminate(int depth,std::vector<Node*>& _objects);
      
      void
      find_plane(Vector3& _min,Vector3& _max,float& _split,Axis& _axis);
      
      
      thread_local static std::vector<StackEntry> stack;
      static const int MAX_STACK_SIZE=40;
      const int MAX_PRIMITIVES_IN_LEAF=6;
      const int MAX_TREE_DEPTH=5;
  };
  
}

#endif

