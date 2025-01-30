#pragma once

#include "Core/SharedData.hpp"

namespace BB {

    /** @brief The base class for a Module. Pure virtual class. */
    class Module {
    public:
        Module() = default;
        virtual ~Module() = default;

        /** @brief Initializes the Module. */
        virtual void Init(SharedData* data) = 0;
        /** @brief Shut's down the Module. */
        virtual void Shutdown() = 0;

        /** @brief Update's the Module. */
        virtual void Update() = 0;

    private:
        Module(const Module&) = delete;
        Module& operator = (const Module&) = delete;

    protected:
        SharedData* p_Data = nullptr; /** @brief The pointer to the shared data where results are saved for GUI. */
        
    };

} // BB
