#pragma once

#include "physical_io/IDigitalPort.hpp"
#include "physical_io/IAnalogPort.hpp"
#include "mower_controller/mower_control_pin_assignments.hpp"
#include "mower_controller/mower_control_state.hpp"

namespace cotsbotics::mower_controller
{
    // Manual control interface for the mower
    class MowerManualControlInputManager
    {
    public:
        /// @brief Constructor
        explicit MowerManualControlInputManager(
            MowerControlPinAssignments::Inputs pins,
            IDigitalPort &left_motor_zero_switch,
            IAnalogPort &left_motor_throtle,
            IDigitalPort &right_motor_zero_switch,
            IAnalogPort &right_motor_throtle,
            IDigitalPort &seat_switch_drive_controls,
            IDigitalPort &seat_switch_blade_controls,
            IDigitalPort &low_speed_drive,
            IDigitalPort &brake_engaged,
            IDigitalPort &low_speed_cut,
            IDigitalPort &blades_enabled);

        void tick();

        MowerControlState const getState() const;

    private:
        MowerControlState m_state;
        /// @brief Pin assignments for the mower control inputs
        /// @{
        IDigitalPort &m_left_motor_zero_switch;
        IAnalogPort &m_left_motor_throtle;
        IDigitalPort &m_right_motor_zero_switch;
        IAnalogPort &m_right_motor_throtle;
        IDigitalPort &m_seat_switch_drive_controls;
        IDigitalPort &m_seat_switch_blade_controls;
        IDigitalPort &m_low_speed_drive;
        IDigitalPort &m_brake_engaged;
        IDigitalPort &m_low_speed_cut;
        IDigitalPort &m_blades_enabled;
        /// @}
    };

}; // cotsbotics::mower_controller