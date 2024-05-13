#pragma once
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <functional>
#include <iostream>

class ThreadPool
{
private:
    ThreadPool();
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

    std::vector<std::thread> pool;
    const int threadNum = std::thread::hardware_concurrency();
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic_bool _stop;

    std::queue<std::packaged_task<void()>> taskQueue;

public:
    ~ThreadPool();
    static ThreadPool &getInstance()
    {
        static ThreadPool _instance;
        return _instance;
    }

    template <typename F, typename... Arg>
    auto assign(F &&f, Arg &&...args) -> std::future<decltype(f(std::forward<Arg>(args)...))>
    {
        using RetType = decltype(f(std::forward<Arg>(args)...));
        auto ptr = std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Arg>(args)...));
        std::future<RetType> fut = ptr->get_future();
        mtx.lock();
        taskQueue.emplace([ptr]
                          { (*ptr)(); });
        mtx.unlock();
        cv.notify_one();
        return fut;
    }
};

inline ThreadPool::ThreadPool()
{
    for (int i = 0; i < threadNum; i++)
    {
        pool.emplace_back([this]
                          {
            std::unique_lock l(mtx,std::defer_lock);
            std::packaged_task<void()> t;
            while (true)
            {
                l.lock();
                cv.wait(l,[this]{return _stop.load() || !taskQueue.empty();});
                if (taskQueue.empty())
                    return;
                t = move(taskQueue.front());
                taskQueue.pop();
                l.unlock();
                t();
            } });
    }
}

inline ThreadPool::~ThreadPool()
{
    _stop = true;
    cv.notify_all();
    for (auto &t : pool)
        if (t.joinable())
            t.join();
}
