#include "Core/Application.hpp"

// std
#include <iostream>

int main(/* int argc, char** argv */) {
    CB::Application* app = new CB::Application;

    try {
        app->Init();

        app->Run();

        app->Shutdown();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;

        delete app;
        return EXIT_FAILURE;
    }

    delete app;
    return EXIT_SUCCESS;
}
