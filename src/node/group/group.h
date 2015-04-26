#ifndef SVIT_GROUP
#define SVIT_GROUP

#include "node/node.h"
#include "node/kdtree.h"

namespace Svit
{
  /**
   * @brief The Group class represents the scene as hierarchy of Nodes with 
   * Solids as leafs. 
   */
	class Group : public Node
	{
		public:
      /**
       * @brief add Node to the hierarchy.
       * @param _node to be added.
       */
			virtual void
			add (Node* _node) = 0;
      
      /**
       * @brief finish the hierarchy construction -> build the kdtree. This 
       * method is called once after all the nodes were inserted into the 
       * hierarchy. 
       */
      virtual void
			finish () = 0;
	};
}

#endif

