#include "ThreadPool.h"
#include "FileProcessor.h"

#include <cstddef>
#include <thread>
#include <mutex>

Counters ThreadPool::run(const std::vector<std::filesystem::path>& files) {
    Counters global_result;

    std::mutex next_mutex;
    std::mutex result_mutex;
	
	size_t next_file = 0;

    unsigned int thread_count = std::thread::hardware_concurrency();
	if(thread_count == 0) { thread_count = 4; }
    std::vector<std::thread> threads;

    for(unsigned int t = 0; t < thread_count; ++t) {
        threads.emplace_back([&]() {
            Counters local;
			
			while(true) {
                std::filesystem::path path;

                {
                    std::lock_guard<std::mutex> lock(next_mutex);
                    if(next_file >= files.size())
                        break;
                    path = files[next_file++];
                }

                FileProcessor::process_file(path, local);
            }

            {
                std::lock_guard<std::mutex> lock(result_mutex);
                global_result.merge(local);
            }

        });
    }

    for(auto& th : threads)
        th.join();

    return global_result;
}

