#include "ThreadPool.h"


void ThreadPool::task_runner()
{
	while (true)
	{
		std::function<void()> task;
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock, [this] { return !tasks.empty() || is_stopped; });
			if (is_stopped && tasks.empty())
			{
				return;
			}
			task = std::move(tasks.front());
			tasks.pop();
		}
		task();
	}

}


void ThreadPool::add_task(std::function<void()> task)
{
	{
		std::unique_lock<std::mutex> lock(mtx);
		tasks.push([this, task]()
			{
				task();

				{
					std::unique_lock<std::mutex> lock(mtx);
					active_tasks--;
				}

				finished_cv.notify_one();
			});
		active_tasks++;
	}

	cv.notify_one();
}


void ThreadPool::wait_for_tasks() 
{
	std::unique_lock<std::mutex> lock(mtx);
	finished_cv.wait(lock, [this] { return tasks.empty() && active_tasks == 0; });
}
