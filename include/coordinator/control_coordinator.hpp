// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "radio_control/radio_controller.hpp"
#include "mower_controller/mower_control_output.hpp"
#include "mower_controller/mower_manual_control_input.hpp"
#include "physical_io/IDigitalInputPort.hpp"
#include "coordinator/control_mode.hpp"
#include "physical_io/AdcManager.hpp"
namespace cotsbotics::coordinator
{
    class ControlCoordinator
    {
    public:
        ControlCoordinator(
            AdcManager& adc_manager,
            radio_control::RadioController &radio_controller,
            mower_controller::MowerManualControlInputManager &manual_control_input_manager,
            mower_controller::MowerControlOutputManager &control_output_manager,
            IDigitalInputPort& remote_control_interlock_a,
            IDigitalInputPort& remote_control_interlock_b);

        /// @brief Run periodic tasks for the control coordinator
        void tick();

        /// @brief Determine if the remote control interlock is engaged (allowing radio or robotic takeover)
        void determineControlInterlock();

        /// @brief if the mower is in remote or robotic control mode, check for manual control kickout conditions.
        /// e.g. seat switch pressed, brake engaged, etc.
        void manualControlKickOutProcessing();

        inline bool isInARoboticOrRemoteControlMode() const
        {
            return _current_control_mode != ControlMode::Manual;
        }

    private:
        AdcManager& _adc_manager;
        radio_control::RadioController &_radio_controller;
        mower_controller::MowerManualControlInputManager &_manual_control_input_manager;
        mower_controller::MowerControlOutputManager &_control_output_manager;
        IDigitalInputPort& _remote_control_interlock_a;
        IDigitalInputPort& _remote_control_interlock_b;
        bool _remote_control_interlock_engaged;
        ControlMode _current_control_mode{ControlMode::Manual};
    };

}; // namespace cotsbotics::coordinator