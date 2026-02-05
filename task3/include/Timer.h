#include <chrono>

class Timer
{
public:

	static void timer_start()
	{
		start = std::chrono::steady_clock::now();
	}

	static void timer_end()
	{
		end = std::chrono::steady_clock::now();
	}

	static double get_time()
	{
		std::chrono::duration<double> elapsed = end - start;
		return elapsed.count();
	}

private:
	static std::chrono::steady_clock::time_point start;
	static std::chrono::steady_clock::time_point end;
};

