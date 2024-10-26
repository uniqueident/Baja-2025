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

    /**
     * @brief The following ThreadPool implementation is heavily based upon Zen Sepiol's implementation at: 
     * https://github.com/ZenSepiol/Dear-ImGui-App-Framework/blob/main/src/lib/thread_pool/thread_pool.hpp
     * 
     * MIT License
     * 
     * Copyright (c) 2022 Zen Sepiol
     * 
     * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
     * and associated documentation files (the "Software"), to deal in the Software without restriction, 
     * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
     * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
     * subject to the following conditions:
     * 
     * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
     * 
     * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
     * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
     * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
     * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
     * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
     * 
     */
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
