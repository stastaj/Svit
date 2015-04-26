#ifndef SVIT_KDTREE
#define SVIT_KDTREE

#include "node/axis.h"
#include "geom/vector.h"
#include "geom/aabb.h"

#include <vector>
#include <limits>


namespace Svit
{  
  
  /**
   * @brief The hash_table_entry struct is for "mailboxing", when the kdtree is
   * traversed for intersection, one primitive can be intersected more than
   * one time -> mailboxing solves this, it saves the intersected solid ids and 
   * ray ids into a hashtable and intersects only primitives not present in the 
   * hash table. 
   */
  struct hash_table_entry
  {
    unsigned int triId, rayId;
  };
  
  /**
   * @brief The KdTreeNode class represents one node of the kdTree. 
   */
  class KdTreeNode{
    public:
      KdTreeNode():axis(Leaf) {}
      
      ~KdTreeNode(){
        if(axis!=Leaf){
          delete left;
          delete right;
        }
        else{
          primitives->clear();
          delete primitives;
        }
      }
      
      KdTreeNode* left;
      KdTreeNode* right;
      Axis axis;
      float split;
      std::vector<Solid*>* primitives;
  };
  
  /**
   * @brief The StackEntry struct is used in traversal of the kdtree.
   */
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
      
      KdTreeNode* build(std::vector<Solid*>& _primitives, Vector3 _min, Vector3 _max,
            int _depth); 
      
      bool
      check_tree(KdTreeNode* node);
      
    private:
      void
      split_primitives(std::vector<Solid*>& _primitives, float _split, Axis _axis,
                       std::vector<Solid*>& _left, std::vector<Solid*>& _right);
      
      bool
      terminate(int depth,std::vector<Solid*>& _objects);
      
      void
      find_plane(Vector3& _min,Vector3& _max,float& _split,Axis& _axis);
      
      inline bool 
      AlreadyIntersected(unsigned int triId, unsigned int rayId) const
      {
        return hash_table[triId & HASH_TABLE_MASK].triId == triId &&
                hash_table[triId & HASH_TABLE_MASK].rayId == rayId;
      }
      
      inline void 
      MarkIntersection(unsigned int triId, unsigned int rayId) const 
      {
        hash_table[triId & HASH_TABLE_MASK].triId = triId;
        hash_table[triId & HASH_TABLE_MASK].rayId = rayId;
      }
      
      
      static const unsigned int MAX_STACK_SIZE=40;
      const int MAX_PRIMITIVES_IN_LEAF=6;
      const int MAX_TREE_DEPTH=5;
      
      static const unsigned int HASH_TABLE_SIZE=64;
      static const unsigned int HASH_TABLE_MASK=HASH_TABLE_SIZE-1;
      
      thread_local static std::vector<hash_table_entry> hash_table;
      thread_local static std::vector<StackEntry> stack;
  };
  
}

#endif

