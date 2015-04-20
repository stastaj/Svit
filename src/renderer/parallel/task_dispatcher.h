#ifndef SVIT_TASK_DISPATCHER
#define SVIT_TASK_DISPATCHER

#include "renderer/settings.h"

#include <vector>
#include <boost/optional.hpp>
#include <mutex>

namespace Svit
{
  typedef int Task;

	class TaskDispatcher
	{
		private:
			std::mutex mutex;
			std::vector<Task> tasks;

		public:
			TaskDispatcher (Settings& _settings);

			boost::optional<Task> 
			get_task ();

			unsigned int index;
	};
}

#endif

