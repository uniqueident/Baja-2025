#include "Core/Application.hpp"

// std
#include <iostream>

int main(/* int argc, char** argv */) {
    // Creating the application dynamically helps keep a smaller stack size.
    CB::Application* app = new CB::Application;

    // The application is initialized, then enters the runtime. Once the runtime is exited, application is shut down before being deleted.
    try {
        app->Init();

        app->Run();

        app->Shutdown();
    }
    catch (const std::exception& e) { // If something start's crashing and burning, destroy it all and there might be a helpful message.
        std::cerr << e.what() << std::endl;

        delete app;
        return EXIT_FAILURE;
    }

    delete app;
    return EXIT_SUCCESS;
}
