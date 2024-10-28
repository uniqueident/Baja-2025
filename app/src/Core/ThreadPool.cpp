#include "ThreadPool.hpp"

namespace CB {

    ThreadPool::ThreadPool(const size_t size) : m_Threads(std::vector<std::thread>(size)), m_BusyThreads(size) {
        for (size_t i = 0; i < size; i++)
            m_Threads[i] = std::thread(ThreadWorker(this));
    }

    ThreadPool::~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_ShutdownRequested = true;
            m_Conditional.notify_all();
        }

        for (size_t i = 0; i < m_Threads.size(); i++) {
            if (m_Threads[i].joinable())
                m_Threads[i].join();
        }
    }

    ThreadPool::ThreadWorker::ThreadWorker(ThreadPool* pool) : p_Pool(pool) { }

    void ThreadPool::ThreadWorker::operator () () {
        std::unique_lock<std::mutex> lock(p_Pool->m_Mutex);

        while (
            !p_Pool->m_ShutdownRequested ||
            (p_Pool->m_ShutdownRequested && !p_Pool->m_Queue.empty())) {
            p_Pool->m_BusyThreads--;
                
            p_Pool->m_Conditional.wait(lock, [this] {
                return this->p_Pool->m_ShutdownRequested || !this->p_Pool->m_Queue.empty();
            });

            p_Pool->m_BusyThreads++;

            if (!this->p_Pool->m_Queue.empty()) {
                auto func = p_Pool->m_Queue.front();

                p_Pool->m_Queue.pop();

                lock.unlock();
                func();
                lock.lock();
            }
        }
    }

} // CB