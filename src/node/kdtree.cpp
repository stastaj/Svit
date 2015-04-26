#include "node/kdtree.h"



namespace Svit
{ 
  
  thread_local std::vector<StackEntry> KdTree::stack(MAX_STACK_SIZE);
  thread_local std::vector<hash_table_entry> KdTree::hash_table(HASH_TABLE_SIZE);
  
  bool 
  KdTree::traverse(const AABB& bb, const Ray& _ray, 
                   Intersection& _intersection) const
  {
    float a,b;
    bool hit=bb.intersect_with_line(_ray, a, b );
    if ( ! hit )
      return false;
    
    //std::vector<StackEntry> stack(30);
    
    KdTreeNode* farChild;
    KdTreeNode* currNode = root; 
    int enPt = 0; 
    stack[enPt].t = std::max(a,0.0f); 
    
    if (a >= 0.0f) 
      stack[enPt].coord = _ray(a);
    else 
      stack[enPt].coord = _ray.origin;
    int exPt = 1; 
    stack[exPt].t = b;
    stack[exPt].coord = _ray(b);
    stack[exPt].node = nullptr; 
    
    float t;
    float t_max=_intersection.t;
    
    while ( currNode != nullptr  ){
      while ( currNode->axis != Leaf ){
        float splitVal = currNode->split;
        Axis axis=currNode->axis;
        
        if (stack[enPt].coord[axis] <= splitVal){
          if (stack[exPt].coord[axis] <= splitVal){
            currNode = currNode->left;
            continue;
          }
          if (stack[exPt].coord[axis] == splitVal){
            currNode = currNode->right;
            continue; 
          }
          farChild = currNode->right;
          currNode = currNode->left;
        }
        else{ /* (stack[enPt].pb[axis] >splitVal) */
          if ( splitVal < stack[exPt].coord[axis]){
            currNode = currNode->right;
            continue;
          }
          farChild = currNode->left;
          currNode = currNode->right;
        }
        /* signed distance to the splitting plane */
        t = (splitVal - _ray.origin[axis]) / _ray.direction[axis];
        /* setup the new exit point */
        int tmp = exPt;
        ++exPt;
        
        /* possibly skip current entry point so not to overwrite the data */
        if (exPt == enPt)
          ++exPt;
        
        stack[exPt].prev = tmp;
        stack[exPt].t = t;
        stack[exPt].node = farChild;
        stack[exPt].coord[axis] = splitVal;
        Axis nextAxis=(Axis)((axis+1) % 3);
        Axis prevAxis=(Axis)((axis+2) % 3);
        stack[exPt].coord[nextAxis] = _ray.origin[nextAxis] +
                                      t * _ray.direction[nextAxis];
        stack[exPt].coord[prevAxis] = _ray.origin[prevAxis] +
                                      t * _ray.direction[prevAxis];
      }
      
      /* current node is the leaf . . . empty or full */
      //"intersect ray with each object in the object list, discarding "
      //"those lying before stack[enPt].t or farther than stack[exPt].t"
      
      bool intersection_found=false;
      _intersection.t=std::min(t_max,stack[exPt].t+RAY_EPSILON);
      for(Solid* solid: *currNode->primitives){
        if(solid->intersect(_ray,_intersection))
          intersection_found=true;
        
      }
      if (intersection_found){
        return true;
      }
      
      /*
      for(Solid* solid: currNode->primitives){
        if( ! AlreadyIntersected(solid->ID,_ray.id)){
          solid->intersect(_ray,_intersection);
          MarkIntersection(solid->ID,_ray.id);
        }
        //intersection_found=true;
      }
      if ( _intersection.t <=  std::min(t_max,stack[exPt].t+RAY_EPSILON)){
        return true;
      }*/
      if(t_max<stack[exPt].t+RAY_EPSILON){
        return false;
      }
      /* pop from the stack */
      enPt = exPt; /* the signed distance intervals are adjacent */      
      currNode = stack[exPt].node;
      exPt = stack[enPt].prev;
    }
    return false;
  }
    
  KdTreeNode*
  KdTree::build(std::vector<Solid*>& _primitives, Vector3 _min, 
                            Vector3 _max, int _depth) 
  {
    KdTreeNode* node=new KdTreeNode();      
    if (terminate( _depth, _primitives)) { 
      node->primitives=new std::vector<Solid*>;
      node->primitives->resize(_primitives.size());
      for(unsigned int i=0;i<_primitives.size();++i){
        (*node->primitives)[i]=_primitives[i];
      }
      node->axis=Leaf;
      node->right=nullptr;
      node->left=nullptr;
      return node;
    }
    find_plane(_min,_max,node->split,node->axis);
    std::vector<Solid*> left;
    std::vector<Solid*> right;
    split_primitives(_primitives, node->split, node->axis, left, right);
    switch (node->axis) { 
      case X:
        node->left=build(left,_min,Vector3(node->split,_max.y,_max.z),_depth+1);
        node->right=build(right,Vector3(node->split,_min.y,_min.z),_max,_depth+1);
        break;
      case Y:
        node->left=build(left,_min,Vector3(_max.x,node->split,_max.z),_depth+1);
        node->right=build(right,Vector3(_min.x,node->split,_min.z),_max,_depth+1);
        break;
      case Z:
        node->left=build(left,_min,Vector3(_max.x,_max.y,node->split),_depth+1);
        node->right=build(right,Vector3(_min.x,_min.y,node->split),_max,_depth+1);
        break;
      default:
        std::cout<<"";  
    }
    return node;
  }
  
  void
  KdTree::split_primitives(std::vector<Solid*>& _primitives, float _split,
                           Axis _axis, std::vector<Solid*>& _left, 
                           std::vector<Solid*>& _right){
    for(Solid* solid:_primitives){
      Node* node=(Node*)solid;
      AABB bb = node->get_aabb();
      if(bb.max[_axis]>_split){
        _right.push_back(solid);
      }
      if(bb.min[_axis]<=_split){
        _left.push_back(solid);
      }
    }
  }
  
  bool
  KdTree::terminate(int depth, std::vector<Solid*>& _primitives){
    return _primitives.size() <= MAX_PRIMITIVES_IN_LEAF || 
        depth >= MAX_TREE_DEPTH;
  }
  
  void
  KdTree::find_plane(Vector3& _min,Vector3& _max,float& _split,Axis& _axis){
    Vector3 v=_max-_min;
    if (std::abs(v.x) > std::abs(v.y)){
      if (std::abs(v.x) > std::abs(v.z)){
        _axis = X;
        _split = (_min.x + _max.x) * 0.5f;
      }
      else{
        _axis = Z;
        _split = (_min.z + _max.z) * 0.5f;
      }
    }
    else {
      if (std::abs(v.y) >= std::abs(v.z)){
        _axis = Y;
        _split = (_min.y + _max.y) * 0.5f;
      }
      else { 
        _axis = Z;
        _split = (_min.z + _max.z) * 0.5f;
      }
    }
  }
  
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
        for(Solid* s: *node->primitives){
          if(s==nullptr)
            return false;
        }
        break;
      default:
        return false;
    }
    return true;
  }
}

