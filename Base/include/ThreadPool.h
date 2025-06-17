// ThreadPool.h
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <future>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();

    // Submit a task and get a future to track it
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::invoke_result<F, Args...>::type>;

    void stop();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stopFlag;
};

// Constructor
inline ThreadPool::ThreadPool(size_t numThreads) : stopFlag(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] {
                        return this->stopFlag || !this->tasks.empty();
                    });

                    if (this->stopFlag && this->tasks.empty())
                        return;

                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

// Destructor
inline ThreadPool::~ThreadPool() {
    stop();
}

// Stop pool and join all threads
inline void ThreadPool::stop() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stopFlag = true;
    }

    condition.notify_all();

    for (std::thread &worker : workers)
        if (worker.joinable())
            worker.join();
}

// Enqueue tasks
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::invoke_result<F, Args...>::type>
{
    using return_type = typename std::invoke_result<F, Args...>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    std::future<return_type> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (stopFlag)
            throw std::runtime_error("Enqueue on stopped ThreadPool");

        tasks.emplace([task]() { (*task)(); });
    }

    condition.notify_one();
    return res;
}
