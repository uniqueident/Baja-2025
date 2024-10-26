/**
 * @file ThreadPool.hpp
 * @author Evan F.
 * @brief The ThreadPool header file. Declares the ThreadPool class members and methods.
 * @version 1.0
 * @date 10-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

// std
#include <mutex>
#include <thread>

#include <future>
#include <functional>
#include <condition_variable>

#include <queue>

namespace CB {

    class ThreadPool {
    public:
        ThreadPool(const size_t size);
        ~ThreadPool();

        template<typename F, typename... Args>
        auto AddTask(F&& f, Args&&... args) -> std::future<decltype(f(args...)) >;

    private:
        mutable std::mutex m_Mutex;
        std::condition_variable m_Conditional;

        std::vector<std::thread> m_Threads;
        int m_BusyThreads = 0;
        bool m_ShutdownRequested = false;

        std::queue<std::function<void()>> m_Queue;

        class ThreadWorker {
        public:
            ThreadWorker(ThreadPool* pool);

            void operator () ();

        private:
            ThreadPool* p_Pool = nullptr;

        };
    };


    template<typename F, typename ...Args>
    inline auto ThreadPool::AddTask(F && f, Args && ...args) -> std::future<decltype(f(args ...)) > {
        auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))() >> (std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        auto wrapper = [taskPtr]() { (*taskPtr)(); };
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Queue.push(wrapper);

            m_Conditional.notify_one();
        }

        return taskPtr->get_future();
    }

}   // CB
