// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IDigitalOutputPort.hpp"
#include "physical_io/IAnalogOutputPort.hpp"
#include "mower_controller/mower_control_pin_assignments.hpp"
#include "mower_controller/mower_control_state.hpp"

namespace cotsbotics::mower_controller
{
    // Output control interface for the mower
    class MowerControlOutputManager
    {
    public:
        /// @brief Constructor
        explicit MowerControlOutputManager(
            IDigitalOutputPort &left_motor_zero_switch,
            IAnalogOutputPort &left_motor_throttle,
            IDigitalOutputPort &right_motor_zero_switch,
            IAnalogOutputPort &right_motor_throttle,
            IDigitalOutputPort &seat_switch_drive_controls,
            IDigitalOutputPort &seat_switch_blade_controls,
            IDigitalOutputPort &low_speed_drive,
            IDigitalOutputPort &brake_engaged,
            IDigitalOutputPort &low_speed_cut,
            IDigitalOutputPort &blades_enabled);

        void setup();

        void tick();

        MowerControlState &getState() ;

    private:
        MowerControlState m_state;
        /// @brief Pin assignments for the mower control inputs
        /// @{
        IDigitalOutputPort &m_left_motor_zero_switch;
        IAnalogOutputPort &m_left_motor_throttle;
        IDigitalOutputPort &m_right_motor_zero_switch;
        IAnalogOutputPort &m_right_motor_throttle;
        IDigitalOutputPort &m_seat_switch_drive_controls;
        IDigitalOutputPort &m_seat_switch_blade_controls;
        IDigitalOutputPort &m_low_speed_drive;
        IDigitalOutputPort &m_brake_engaged;
        IDigitalOutputPort &m_low_speed_cut;
        IDigitalOutputPort &m_blades_enabled;
        /// @}
    };

}; // cotsbotics::mower_controller