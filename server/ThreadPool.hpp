//
// Created by Arjun Varma on 06/01/26.
//

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP



#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads, size_t maxQueueSize);
    ~ThreadPool();

    bool submit(std::function<void()> task);
    void shutdown();

private:
    void workerLoop();

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    size_t maxQueueSize;

    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> stopping{false};
};



#endif //THREADPOOL_HPP
