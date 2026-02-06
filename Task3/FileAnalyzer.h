#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

template<typename T>
class SafeQueue {
private:
     std::queue<T> queue;
     std::mutex mtx;
     std::condition_variable cv;
     bool finished = false;

public:
     void push(T value) {
          std::lock_guard<std::mutex> lock(mtx);
          queue.push(std::move(value));
          cv.notify_one();
     }

     bool pop(T& value) {
          std::unique_lock<std::mutex> lock(mtx);
          cv.wait(lock, [this] { return !queue.empty() || finished; });
          if (queue.empty()) return false;
          value = std::move(queue.front());
          queue.pop();
          return true;
     }

     void set_finished() {
          std::lock_guard<std::mutex> lock(mtx);
          finished = true;
          cv.notify_all();
     }
};

extern std::atomic<int> total_files;
extern std::atomic<long long> total_blank;
extern std::atomic<long long> total_comment;
extern std::atomic<long long> total_code;

void analyze_file(const std::string& path);
