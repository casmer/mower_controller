
#include "mower_controller/mower_manual_control_input.hpp"

namespace cotsbotics
{
    namespace mower_controller
    {

        MowerManualControlInputManager::MowerManualControlInputManager(
            IDigitalInputPort &left_motor_zero_switch,
            IAnalogInputPort &left_motor_throttle,
            IDigitalInputPort &right_motor_zero_switch,
            IAnalogInputPort &right_motor_throttle,
            IDigitalInputPort &seat_switch_drive_controls,
            IDigitalInputPort &seat_switch_blade_controls,
            IDigitalInputPort &low_speed_drive,
            IDigitalInputPort &brake_engaged,
            IDigitalInputPort &low_speed_cut,
            IDigitalInputPort &blades_enabled) : m_left_motor_zero_switch(left_motor_zero_switch),
                                                 m_left_motor_throttle(left_motor_throttle),
                                                 m_right_motor_zero_switch(right_motor_zero_switch),
                                                 m_right_motor_throttle(right_motor_throttle),
                                                 m_seat_switch_drive_controls(seat_switch_drive_controls),
                                                 m_seat_switch_blade_controls(seat_switch_blade_controls),
                                                 m_low_speed_drive(low_speed_drive),
                                                 m_brake_engaged(brake_engaged),
                                                 m_low_speed_cut(low_speed_cut),
                                                 m_blades_enabled(blades_enabled) {
                                                 };

        void MowerManualControlInputManager::setup()
        {

            m_left_motor_zero_switch.setup();
            m_left_motor_throttle.setup();
            m_right_motor_zero_switch.setup();
            m_right_motor_throttle.setup();
            m_seat_switch_drive_controls.setup();
            m_seat_switch_blade_controls.setup();
            m_low_speed_drive.setup();
            m_brake_engaged.setup();
            m_low_speed_cut.setup();
            m_blades_enabled.setup();
        };

        void MowerManualControlInputManager::tick()
        {

            MowerControlState new_state;
            new_state.left_motor.throttle_position = m_left_motor_throttle.read();
            new_state.left_motor.zero_switch = m_left_motor_zero_switch.read();
            new_state.right_motor.throttle_position = m_right_motor_throttle.read();
            new_state.right_motor.zero_switch = m_right_motor_zero_switch.read();
            new_state.seat_switch_drive = m_seat_switch_drive_controls.read();
            new_state.seat_switch_blade = m_seat_switch_blade_controls.read();
            new_state.low_speed_drive = m_low_speed_drive.read();
            new_state.brake_engaged = m_brake_engaged.read();
            new_state.low_speed_cut = m_low_speed_cut.read();
            new_state.blades_enabled = m_blades_enabled.read();
            m_state = new_state;
        };

        MowerControlState const &MowerManualControlInputManager::getState() const
        {
            return m_state;
        };

    }; // mower_controller
}; // cotsbotics