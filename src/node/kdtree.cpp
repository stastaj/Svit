#include "node/kdtree.h"

namespace Svit
{ 
  
  thread_local std::vector<StackEntry> KdTree::stack(50);
  
  
  bool 
  KdTree::traverse(KdTreeNode* root, const AABB& bb, const Ray& _ray, 
                   Intersection& _intersection)
  {
    float a,b;
    bool hit=bb.intersect_with_line(_ray, a, b );
    if ( ! hit )
      return false;
    
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
    while ( currNode != nullptr ){
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
        --exPt;
        
        /* possibly skip current entry point so not to overwrite the data */
        if (exPt == enPt)
          --exPt;
        
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
      //_ray.t_min=stack[enPt].t;
      _intersection.t=std::min(stack[exPt].t,_intersection.t);
      for(Node* node: currNode->primitives){
        if(node->intersect(_ray,_intersection)){
          intersection_found=true;
        }
      }
      if ( intersection_found ){
        return true;
      }
      /* pop from the stack */
      enPt = exPt; /* the signed distance intervals are adjacent */      
      currNode = stack[exPt].node;
      exPt = stack[enPt].prev;
    }
    return false;
  }
  
  KdTreeNode* 
  KdTree::build(std::vector<Node*>& _primitives,Vector3 _min, 
                            Vector3 _max, int _depth) 
  {
    KdTreeNode* node=new KdTreeNode();      
    if (terminate( _depth, _primitives)) { 
      node->primitives=_primitives;
      node->axis=Leaf;
      return node;
    }
    find_plane(_min,_max,node->split,node->axis);
    std::vector<Node*> left;
    std::vector<Node*> right;
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
    }
    return node;
  }
  
  void
  KdTree::split_primitives(std::vector<Node*> _primitives,float _split,Axis _axis,
                           std::vector<Node*> _left, std::vector<Node*> _right){
    for(Node* node:_primitives){
      AABB bb = node->get_aabb();
      switch(_axis){
        case X:
          if(bb.max.x <= _split){
            _left.push_back(node);
          }
          else if(bb.min.x >= _split){
            _right.push_back(node);
          }
          else{
            _left.push_back(node);
            _right.push_back(node);
          }
          break;
        case Y:
          if(bb.max.y <= _split){
            _left.push_back(node);
          }
          else if(bb.min.y >= _split){
            _right.push_back(node);
          }
          else{
            _left.push_back(node);
            _right.push_back(node);
          }
          break;
        case Z:
          if(bb.max.z <= _split){
            _left.push_back(node);
          }
          else if(bb.min.z >= _split){
            _right.push_back(node);
          }
          else{
            _left.push_back(node);
            _right.push_back(node);
          }
          break;
      }
    }
  }
  
  bool
  KdTree::terminate(int depth, std::vector<Node*>& _primitives){
    return _primitives.size() <= 1 || depth >= 20;
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
}

