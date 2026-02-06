#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <condition_variable>
#include <functional>
#include <iostream>
#include <queue>
#include <random>
#include <shared_mutex>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <string>

inline std::mutex printMutex;
template<typename... Args>
void print_sync(Args&&... args) {
    std::lock_guard<std::mutex> lock(printMutex);
    (std::cout << ... << std::forward<Args>(args)) << std::endl;
}

struct Task {
    int id;
    std::function<void()> func;

    explicit Task(int id = 0, std::function<void()> func = [] {})
        : id(id), func(std::move(func)) {}

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;
    Task(Task&&) = default;
    Task& operator=(Task&&) = default;
};

class tasksQueue {
    std::shared_mutex mutexQ;
    std::queue<Task> tasks;
public:
    tasksQueue() = default;
    ~tasksQueue();

    tasksQueue(const tasksQueue&) = delete;
    tasksQueue& operator=(const tasksQueue&) = delete;
    tasksQueue(tasksQueue&&) = delete;
    tasksQueue& operator=(tasksQueue&&) = delete;

    bool empty();
    std::size_t size();
    void clear();
    bool pop(Task& task);
    bool push(int taskId, std::function<void()> func);
};

class threadPool {
    std::vector<std::thread> workers;
    tasksQueue queue1;
    tasksQueue queue2;

    std::mutex q1_mutex;
    std::condition_variable q1_cv;
    std::mutex q2_mutex;
    std::condition_variable q2_cv;

    //std::mutex addTask;
    std::mutex poolStateMutex; // for stop/terminate/pause

    std::atomic<bool> terminated{false};
    std::atomic<bool> initialized{false};
    std::atomic<bool> paused{false};

    std::atomic<int> totalTasks{0};

    bool working_unsafe() const;

public:
    bool working();
    void initialize(int workerNumber);
    void add_task(std::function<void()> func, int prefferedQ = -1);
    void routine(int worker_id, const char* queue_name, tasksQueue& assignedQueue, std::mutex& assignedMutex, std::condition_variable& assignedCV, tasksQueue& otherQueue);
    void pause();
    void unpause();
    void terminate();
};

#endif
