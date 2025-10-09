#pragma once

#include "physical_io/IDigitalInputPort.hpp"
#include "physical_io/IAnalogInputPort.hpp"
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
            IDigitalInputPort &left_motor_zero_switch,
            IAnalogInputPort &left_motor_throtle,
            IDigitalInputPort &right_motor_zero_switch,
            IAnalogInputPort &right_motor_throtle,
            IDigitalInputPort &seat_switch_drive_controls,
            IDigitalInputPort &seat_switch_blade_controls,
            IDigitalInputPort &low_speed_drive,
            IDigitalInputPort &brake_engaged,
            IDigitalInputPort &low_speed_cut,
            IDigitalInputPort &blades_enabled);

        void tick();

        MowerControlState const &getState() const;

    private:
        MowerControlState m_state;
        /// @brief Pin assignments for the mower control inputs
        /// @{
        IDigitalInputPort &m_left_motor_zero_switch;
        IAnalogInputPort &m_left_motor_throtle;
        IDigitalInputPort &m_right_motor_zero_switch;
        IAnalogInputPort &m_right_motor_throtle;
        IDigitalInputPort &m_seat_switch_drive_controls;
        IDigitalInputPort &m_seat_switch_blade_controls;
        IDigitalInputPort &m_low_speed_drive;
        IDigitalInputPort &m_brake_engaged;
        IDigitalInputPort &m_low_speed_cut;
        IDigitalInputPort &m_blades_enabled;
        /// @}
    };

}; // cotsbotics::mower_controller