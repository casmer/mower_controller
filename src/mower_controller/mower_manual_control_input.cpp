
#include "mower_controller/mower_manual_control_input.hpp"

namespace cotsbotics
{
    namespace mower_controller
    {
       
             MowerManualControlInputManager::MowerManualControlInputManager(
                IDigitalPort &left_motor_zero_switch,
                IAnalogPort &left_motor_throtle,
                IDigitalPort &right_motor_zero_switch,
                IAnalogPort &right_motor_throtle,
                IDigitalPort &seat_switch_drive_controls,
                IDigitalPort &seat_switch_blade_controls,
                IDigitalPort &low_speed_drive,
                IDigitalPort &brake_engaged,
                IDigitalPort &low_speed_cut,
                IDigitalPort &blades_enabled) : m_left_motor_zero_switch(left_motor_zero_switch),
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
                m_left_motor_zero_switch.tick();
                m_left_motor_throtle.tick();
                m_right_motor_zero_switch.tick();
                m_right_motor_throtle.tick();
                m_seat_switch_drive_controls.tick();
                m_seat_switch_blade_controls.tick();
                m_low_speed_drive.tick();
                m_brake_engaged.tick();
                m_low_speed_cut.tick();
                m_blades_enabled.tick();

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