
#include "mower_controller/mower_control_output.hpp"

namespace cotsbotics
{
    namespace mower_controller
    {

        MowerControlOutputManager::MowerControlOutputManager(
            IDigitalOutputPort &left_motor_zero_switch,
            IAnalogOutputPort &left_motor_throttle,
            IDigitalOutputPort &right_motor_zero_switch,
            IAnalogOutputPort &right_motor_throttle,
            IDigitalOutputPort &seat_switch_drive_controls,
            IDigitalOutputPort &seat_switch_blade_controls,
            IDigitalOutputPort &low_speed_drive,
            IDigitalOutputPort &brake_engaged,
            IDigitalOutputPort &low_speed_cut,
            IDigitalOutputPort &blades_enabled) : m_left_motor_zero_switch(left_motor_zero_switch),
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

        void MowerControlOutputManager::setup()
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

        void MowerControlOutputManager::tick()
        {
            m_left_motor_throttle.write(m_state.left_motor.throttle_position);
            m_left_motor_zero_switch.write(toBool(m_state.left_motor.zero_switch));
            m_right_motor_throttle.write(m_state.right_motor.throttle_position);
            m_right_motor_zero_switch.write(toBool(m_state.right_motor.zero_switch));
            m_seat_switch_drive_controls.write(toBool(m_state.seat_switch_drive));
            m_seat_switch_blade_controls.write(toBool(m_state.seat_switch_blade));
            m_low_speed_drive.write(toBool(m_state.low_speed_drive));
            m_brake_engaged.write(toBool(m_state.brake_engaged));
            m_low_speed_cut.write(toBool(m_state.low_speed_cut));
            m_blades_enabled.write(toBool(m_state.blades_enabled));
        };

        MowerControlState &MowerControlOutputManager::getState()
        {
            return m_state;
        };

    }; // mower_controller
}; // cotsbotics