#ifndef SVIT_GROUP
#define SVIT_GROUP

#include "node/node.h"
#include "node/kdtree.h"

namespace Svit
{
	class Group : public Node
	{
		public:
			virtual void
			add (Node* _node) = 0;
      
      virtual void
			build_kdtree () = 0;
	};
}

#endif

