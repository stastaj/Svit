#include "model.h"
#include "node/group/group.h"           // for Group
#include "node/solid/solid.h"           // for Solid

namespace Svit
{
	void
	Model::add_to_group (Group& group)
	{
		for (auto solid : solids)
			group.add(solid);
	}
}

