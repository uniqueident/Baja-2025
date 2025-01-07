#pragma once

// std
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

namespace BB {

    class ThreadRunner {
    public:
        ThreadRunner() = default;
        ~ThreadRunner() = default;

        inline void AddMethod(std::function<void()> method, int interval) {
            this->m_Methods.emplace_back(std::pair(method, interval));
        }

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
        inline void Stop() {
            this->m_Running = false;
        }

    private:
        bool m_Running;

        std::vector<std::pair<std::function<void()>, int>> m_Methods;

    };  // ThreadRunner

}   // BB