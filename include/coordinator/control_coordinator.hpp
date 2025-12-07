// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "radio_control/radio_controller.hpp"
#include "mower_controller/mower_control_output.hpp"
#include "mower_controller/mower_manual_control_input.hpp"


namespace cotsbotics::coordinator
{
    class ControlCoordinator
    {
    public:
        ControlCoordinator(
            radio_control::RadioController &radio_controller,
            mower_controller::MowerManualControlInputManager &manual_control_input_manager,
            mower_controller::MowerControlOutputManager &control_output_manager);

        /// @brief Run periodic tasks for the control coordinator
        void tick();

    private:
        radio_control::RadioController &_radio_controller;
        mower_controller::MowerManualControlInputManager &_manual_control_input_manager;
        mower_controller::MowerControlOutputManager &_control_output_manager;
    };

}; // namespace cotsbotics::coordinator