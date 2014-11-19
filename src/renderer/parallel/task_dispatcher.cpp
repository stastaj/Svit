#include "renderer/parallel/task_dispatcher.h"

#include <cmath>
#include <stdio.h>

namespace Svit
{
	TaskDispatcher::TaskDispatcher (Settings& _settings)
	{
    for (int i = 0; i < _settings.iterations; ++i)

		{
      Task task=i;
			tasks.push_back(task);
		}

		index = 0;
	}

	boost::optional<Task> 
	TaskDispatcher::get_task ()
	{
		std::lock_guard<std::mutex> guard(mutex);

		if (index != tasks.size())
			return boost::optional<Task>(tasks[index++]);
		else
			return boost::optional<Task>();
	}
}

