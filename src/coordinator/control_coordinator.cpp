// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "coordinator/control_coordinator.hpp"

namespace cotsbotics
{
    namespace coordinator
    {

        ControlCoordinator::ControlCoordinator(
            AdcManager &adc_manager,
            radio_control::RadioController &radio_controller,
            mower_controller::MowerManualControlInputManager &manual_control_input_manager,
            mower_controller::MowerControlOutputManager &control_output_manager,
            ControlInterlock &remote_control_interlock) : _adc_manager(adc_manager),
                                                          _radio_controller(radio_controller),
                                                          _manual_control_input_manager(manual_control_input_manager),
                                                          _control_output_manager(control_output_manager),
                                                          _remote_control_interlock(remote_control_interlock) {
                                                          };

        mower_controller::MowerSwitch ControlCoordinator::fromRadioSwitch(cotsbotics::radio_control::RadioSwitch radio_switch)
        {
            mower_controller::MowerSwitch mower_switch = mower_controller::MowerSwitch::OPEN;
            switch (radio_switch)
            {
            case radio_control::RadioSwitch::RS_HIGH:
                mower_switch = mower_controller::MowerSwitch::OPEN;
                break;
            case radio_control::RadioSwitch::RS_MIDDLE:
                mower_switch = mower_controller::MowerSwitch::OPEN;
                break;
            case radio_control::RadioSwitch::RS_LOW:
                mower_switch = mower_controller::MowerSwitch::CLOSED;
                break;
            }
            return mower_switch;
        }

        ControlMode controlModeFromRadioSwitch(cotsbotics::radio_control::RadioSwitch radio_switch)
        {
            ControlMode mode = ControlMode::Manual;

            switch (radio_switch)
            {
            case radio_control::RadioSwitch::RS_LOW:
                mode = ControlMode::Robotic;
                break;
            case radio_control::RadioSwitch::RS_MIDDLE:
                mode = ControlMode::Remote;
                break;
            case radio_control::RadioSwitch::RS_HIGH:
                mode = ControlMode::Manual;
                break;
            }
            return mode;
        }

        void ControlCoordinator::tick()
        {

            _remote_control_interlock.tick();
            determineControlInterlock();
            _manual_control_input_manager.tick();
            _adc_manager.tick();
            _radio_controller.tick();

            mower_controller::MowerControlState &output_control_state = _control_output_manager.getState();
            mower_controller::MowerControlState const &manual_state = _manual_control_input_manager.getState();

            if (_current_control_mode == ControlMode::Remote)
            {
                const radio_control::RadioControlState &radio_state = _radio_controller.getState();
                // Map radio control state to mower control state
                output_control_state.left_motor.throttle_position = radio_state.left_throttle_position;
                output_control_state.right_motor.throttle_position = radio_state.right_throttle_position;
                output_control_state.left_motor.zero_switch = fromRadioSwitch(radio_state.zero_switch);
                output_control_state.right_motor.zero_switch = fromRadioSwitch(radio_state.zero_switch);
                output_control_state.seat_switch_drive = fromRadioSwitch(radio_state.seat_switch_drive);
                output_control_state.seat_switch_blade = fromRadioSwitch(radio_state.seat_switch_blade);
                output_control_state.low_speed_drive = fromRadioSwitch(radio_state.low_speed_drive);
                output_control_state.low_speed_cut = fromRadioSwitch(radio_state.low_speed_cut);
                output_control_state.blades_enabled = fromRadioSwitch(radio_state.blades_enabled);
                output_control_state.brake_engaged = _manual_control_input_manager.getState().brake_engaged;
                // Handle remote control mode
            }
            else
            {
                // Handle manual control mode
                // Here you can add logic to decide how to coordinate between radio control and manual control
                // For simplicity, let's assume manual control has priority over radio control

                output_control_state = manual_state;
            }

            // Always update brake engaged from manual input
            output_control_state.brake_engaged = manual_state.brake_engaged;

            _control_output_manager.tick();
        };

        void ControlCoordinator::determineControlInterlock()
        {

            switch (_remote_control_interlock.interlockState())
            {
            case InterlockState::Disengaged:
                _current_control_mode = ControlMode::Manual;
                break;  
            case InterlockState::Engaged:
                // let radio control decide
                if (_radio_controller.getState().failsafe)
                {
                    _current_control_mode = ControlMode::Manual;
                }
                else
                {
                    _current_control_mode = controlModeFromRadioSwitch(_radio_controller.getState().control_mode);

                }
                break;
            case InterlockState::Fault:
                _current_control_mode = ControlMode::Manual;    
                //TODO: Need to add fault reporting here somehow.
                break;
            case InterlockState::Unknown:
                _current_control_mode = ControlMode::Manual;    
                break;
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