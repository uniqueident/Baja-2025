/**
 * @file GUIModule.hpp
 * @author Evan F.
 * @brief The GUIModule header file. Declares the GUIModule class members and methods.
 * @version 1.0
 * @date 10-25-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include "../moduletemplate.hpp"

namespace CB {

    class GUIModule : public Module {
    public:
        GUIModule() = default;
        virtual ~GUIModule() = default;

        virtual void Init(SharedData* data) override;
        virtual void Shutdown() override;

        virtual void Update() override;

    private:
        GUIModule(const GUIModule&) = delete;
        GUIModule operator = (const GUIModule&) = delete;



    };

}   // CB
