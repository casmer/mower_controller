// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>


#include "coordinator/control_coordinator.hpp"

namespace cotsbotics
{
    namespace coordinator
    {

        ControlCoordinator::ControlCoordinator(
            AdcManager& adc_manager,
            radio_control::RadioController &radio_controller,
            mower_controller::MowerManualControlInputManager &manual_control_input_manager,
            mower_controller::MowerControlOutputManager &control_output_manager,
            ControlInterlock& remote_control_interlock) : _adc_manager(adc_manager),
                                                           _radio_controller(radio_controller),
                                                           _manual_control_input_manager(manual_control_input_manager),
                                                           _control_output_manager(control_output_manager),
                                                           _remote_control_interlock(remote_control_interlock) {
                                                           };

        void ControlCoordinator::tick()
        {

            _remote_control_interlock.tick();
            determineControlInterlock();
            _manual_control_input_manager.tick();
            _adc_manager.tick();
            _radio_controller.tick();

            
            if (_current_control_mode == ControlMode::Remote)
            {
                auto radio_state = _radio_controller.getState();
                mower_controller::MowerControlState &control_state = _control_output_manager.getState();
                // Map radio control state to mower control state
                control_state.left_motor.throttle_position = radio_state.left_throttle_position;   
                control_state.right_motor.throttle_position = radio_state.right_throttle_position;
                control_state.seat_switch_drive = static_cast<mower_controller::MowerSwitch>(radio_state.seat_switch_drive);
                control_state.seat_switch_blade = static_cast<mower_controller::MowerSwitch>(radio_state.seat_switch_blade);
                control_state.low_speed_drive = static_cast<mower_controller::MowerSwitch>(radio_state      .low_speed_drive);
                control_state.low_speed_cut = static_cast<mower_controller::MowerSwitch>(radio_state.low_speed_cut);
                control_state.blades_enabled = static_cast<mower_controller::MowerSwitch>(radio_state.blades_enabled);
                control_state.brake_engaged = static_cast<mower_controller::MowerSwitch>(radio_state.brake_engaged);
                // Handle remote control mode
            }
            else
            {
                // Handle manual control mode
                // Here you can add logic to decide how to coordinate between radio control and manual control
                // For simplicity, let's assume manual control has priority over radio control
                
                mower_controller::MowerControlState const &manual_state = _manual_control_input_manager.getState();
                _control_output_manager.getState() = manual_state;
            }

            _control_output_manager.tick();
        };

        

        void ControlCoordinator::determineControlInterlock()
        {
            
            if (_remote_control_interlock.interlockState() != InterlockState::Engaged)
            {
                _current_control_mode = ControlMode::Manual;
            }
            else
            {
                //do nothing, let radio control decide
            } 
            if (_remote_control_interlock.interlockState() == InterlockState::Fault)
            {
                // Fault condition, both interlocks the same
                //TODO: do something _interlock_error = true;
            }
        };

        void ControlCoordinator::manualControlKickOutProcessing()
        {
            if (_current_control_mode == ControlMode::Remote || _current_control_mode == ControlMode::Robotic)
            {
                mower_controller::MowerControlState const &manual_state = _manual_control_input_manager.getState();
                // Check for kickout conditions
                if (manual_state.seat_switch_drive == mower_controller::MowerSwitch::CLOSED ||
                    manual_state.brake_engaged == mower_controller::MowerSwitch::CLOSED)
                {
                    // Kick out to manual control
                    _current_control_mode = ControlMode::Manual;
                }
            }
        };
    } // namespace coordinator
} // namespace cotsbotics   