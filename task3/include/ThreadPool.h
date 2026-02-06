#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>


class ThreadPool
{
public:
	ThreadPool()
	{
		thread_num = std::thread::hardware_concurrency();
		for (unsigned int i = 0; i < thread_num; ++i)
		{
			workers.emplace_back(&ThreadPool:: task_runner, this);
		}
	}

	~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(mtx);
			is_stopped = true;
		}

		cv.notify_all();

		for (auto& th : workers)
		{
			th.join();
		}

	}

	void add_task(std::function<void()> task);
	void wait_for_tasks();

private:
	void task_runner();

private:
	size_t thread_num;
	std::vector<std::thread> workers;
	std::queue<std::function<void()>> tasks;
	std::mutex mtx;
	std::condition_variable cv;
	bool is_stopped = false;
	std::atomic<int> active_tasks{ 0 };
	std::condition_variable finished_cv;
};