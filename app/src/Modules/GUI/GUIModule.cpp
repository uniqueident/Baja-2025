#include "GUIModule.hpp"

#include <QtWidgets/qapplication.h>
#include <QtWidgets/qwidget.h>

namespace CB {

    void GUIModule::Init(SharedData * data) {
        p_Data = data;

        int args = 0;
        char** argv = nullptr;
        p_Application = new QApplication(args, argv);

        p_Window = new QWidget;
        p_Window->setWindowTitle("Buggie Bug");
        p_Window->resize(400, 300);
        p_Window->show();

        QApplication::exec();
    }

    void GUIModule::Shutdown() {
        delete p_Window;
        delete p_Application;
    }

    void GUIModule::Update() {

    }

}   // CB
