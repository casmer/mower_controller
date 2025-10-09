
#include "mower_controller/mower_manual_control_input.hpp"

namespace cotsbotics
{
    namespace mower_controller
    {
       
             MowerManualControlInputManager::MowerManualControlInputManager(
                IDigitalInputPort &left_motor_zero_switch,
                IAnalogInputPort &left_motor_throtle,
                IDigitalInputPort &right_motor_zero_switch,
                IAnalogInputPort &right_motor_throtle,
                IDigitalInputPort &seat_switch_drive_controls,
                IDigitalInputPort &seat_switch_blade_controls,
                IDigitalInputPort &low_speed_drive,
                IDigitalInputPort &brake_engaged,
                IDigitalInputPort &low_speed_cut,
                IDigitalInputPort &blades_enabled) : m_left_motor_zero_switch(left_motor_zero_switch),
                                                m_left_motor_throtle(left_motor_throtle),
                                                m_right_motor_zero_switch(right_motor_zero_switch),
                                                m_right_motor_throtle(right_motor_throtle),
                                                m_seat_switch_drive_controls(seat_switch_drive_controls),
                                                m_seat_switch_blade_controls(seat_switch_blade_controls),
                                                m_low_speed_drive(low_speed_drive),
                                                m_brake_engaged(brake_engaged),
                                                m_low_speed_cut(low_speed_cut),
                                                m_blades_enabled(blades_enabled)
            {
            };

            void MowerManualControlInputManager::tick()
            {
                m_state.left_motor.throtle_position = m_left_motor_throtle.read();
                m_state.left_motor.zero_switch = m_left_motor_zero_switch.read();
                m_state.right_motor.throtle_position = m_right_motor_throtle.read();
                m_state.right_motor.zero_switch = m_right_motor_zero_switch.read();
                m_state.seat_switch_drive = m_seat_switch_drive_controls.read();
                m_state.seat_switch_blade = m_seat_switch_blade_controls.read();
                m_state.low_speed_drive = m_low_speed_drive.read();
                m_state.brake_engaged = m_brake_engaged.read();
                m_state.low_speed_cut = m_low_speed_cut.read();
                m_state.blades_enabled = m_blades_enabled.read();
            };

            MowerControlState const &MowerManualControlInputManager::getState() const
            {
                return m_state;
            };

       

    }; // mower_controller
}; // cotsbotics