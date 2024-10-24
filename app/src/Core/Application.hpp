#pragma once
//ensures header is included do not include again.
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
