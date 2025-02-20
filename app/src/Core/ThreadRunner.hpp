#pragma once

// std
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

namespace BB {

    class ThreadRunner {
    public:
        inline ThreadRunner() : m_Running(false), m_Methods(0) { }
        ~ThreadRunner() = default;

        /**
         * @brief Add's a class method to call during the thread update loop.
         * 
         * @param method The class method to call.
         * @param interval The time interval to wait after the call in milliseconds.
         */
        inline void AddMethod(std::function<void()> method, int interval) {
            this->m_Methods.emplace_back(std::pair(method, interval));
        }

        /**
         * @brief Begins the thread runtime.
         * 
         */
        inline void Start() {
            this->m_Running = true;

            std::thread t([&]() {
                while (this->m_Running) {
                    for (const auto& method : this->m_Methods) {
                        method.first();

                        std::cout << "Running Separate Thread" << std::endl;

                        std::this_thread::sleep_for(std::chrono::milliseconds(method.second));
                    }
                }
            });

            t.detach();
        }
        /**
         * @brief Ends the thread runtime.
         * 
         */
        inline void Stop() {
            this->m_Running = false;
        }

    private:
        bool m_Running; /** @brief Records if the thread runtime is active. */

        std::vector<std::pair<std::function<void()>, int>> m_Methods; /** @brief Stores the function methods and the wait time as a pair.  */

    };  // ThreadRunner

}   // BB