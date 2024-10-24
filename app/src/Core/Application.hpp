#pragma once

namespace CB {

    class Application {
    public:
        void Init();
        void Shutdown();

        void Run();

        void RequestShutdown();

    private:
        bool m_Running = false;
    };

}   // CB
