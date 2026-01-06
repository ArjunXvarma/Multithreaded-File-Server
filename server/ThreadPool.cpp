//
// Created by Arjun Varma on 06/01/26.
//

#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t numThreads, size_t maxQueueSize)
    : maxQueueSize(maxQueueSize) {

    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerLoop, this);
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

bool ThreadPool::submit(std::function<void()> task) {
    std::lock_guard<std::mutex> lock(mutex);

    if (stopping || tasks.size() >= maxQueueSize) {
        return false;
    }

    tasks.push(std::move(task));
    cv.notify_one();
    return true;
}

void ThreadPool::shutdown() {
    stopping.store(true);
    cv.notify_all();

    for (auto& t : workers) {
        if (t.joinable()) {
            t.join();
        }
    }
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [&] {
                return stopping || !tasks.empty();
            });

            if (stopping && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}
