#include "GUIModule.hpp"

#include <QApplication>
#include <QPushButton>

namespace CB {

    void GUIModule::Init(SharedData * data) {
        p_Data = data;

        int args = 0;
        QApplication app(args, nullptr);
    }

    void GUIModule::Shutdown() {
    }

    void GUIModule::Update() {
    }

}   // CB
