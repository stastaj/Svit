#ifndef SVIT_MODEL
#define SVIT_MODEL

#include <list>                         // for list
#include <string>

namespace Svit
{
  class Group; 
  class Solid; 
  
  /**
   * @brief The Model class is used for scene serialization and deserialization. 
   */
	class Model
	{
		public:
			std::list<Solid*> solids;

			virtual bool
			load (std::string _filename) = 0;

			virtual void
			add_to_group (Group& group);
	};
}

#endif

