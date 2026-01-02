// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include <gmock/gmock.h>
#include <mock/MockAnalogInputPort.hpp>
#include <mock/MockDigitalInputPort.hpp>
#include "mower_controller/mower_manual_control_input.hpp"

using namespace cotsbotics::mower_controller;
TEST(mower_manual_control_input, tick)
{
MockDigitalInputPort left_motor_zero_switch;
MockAnalogInputPort left_motor_throttle;
MockDigitalInputPort right_motor_zero_switch;
MockAnalogInputPort right_motor_throttle;
MockDigitalInputPort seat_switch_drive_controls;
MockDigitalInputPort seat_switch_blade_controls;
MockDigitalInputPort low_speed_drive;
MockDigitalInputPort brake_engaged;
MockDigitalInputPort low_speed_cut;
MockDigitalInputPort blades_enabled;

MowerControlState expected_state;
expected_state.left_motor.throttle_position = 0.5f;
expected_state.left_motor.zero_switch = MowerSwitch::OPEN;
expected_state.right_motor.throttle_position = -0.5f;    
expected_state.right_motor.zero_switch = MowerSwitch::CLOSED;
expected_state.seat_switch_drive = MowerSwitch::OPEN;
expected_state.seat_switch_blade = MowerSwitch::CLOSED;
expected_state.low_speed_drive = MowerSwitch::OPEN;
expected_state.brake_engaged = MowerSwitch::CLOSED;
expected_state.low_speed_cut = MowerSwitch::OPEN;
expected_state.blades_enabled = MowerSwitch::CLOSED;

MowerManualControlInputManager manager(
    left_motor_zero_switch,
    left_motor_throttle,
    right_motor_zero_switch,
    right_motor_throttle,
    seat_switch_drive_controls,
    seat_switch_blade_controls,
    low_speed_drive,
    brake_engaged,
    low_speed_cut,
    blades_enabled);

EXPECT_CALL(left_motor_zero_switch, setup()).Times(1);
EXPECT_CALL(left_motor_throttle, setup()).Times(1);
EXPECT_CALL(right_motor_zero_switch, setup()).Times(1);
EXPECT_CALL(right_motor_throttle, setup()).Times(1);
EXPECT_CALL(seat_switch_drive_controls, setup()).Times(1);
EXPECT_CALL(seat_switch_blade_controls, setup()).Times(1);
EXPECT_CALL(low_speed_drive, setup()).Times(1);
EXPECT_CALL(brake_engaged, setup()).Times(1);
EXPECT_CALL(low_speed_cut, setup()).Times(1);
EXPECT_CALL(blades_enabled, setup()).Times(1);

manager.setup();

EXPECT_CALL(left_motor_zero_switch, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.left_motor.zero_switch)));
EXPECT_CALL(left_motor_throttle, read()).Times(1).WillOnce(testing::Return(expected_state.left_motor.throttle_position));
EXPECT_CALL(right_motor_zero_switch, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.right_motor.zero_switch)));
EXPECT_CALL(right_motor_throttle, read()).Times(1).WillOnce(testing::Return(expected_state.right_motor.throttle_position));
EXPECT_CALL(seat_switch_drive_controls, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.seat_switch_drive)));
EXPECT_CALL(seat_switch_blade_controls, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.seat_switch_blade)));
EXPECT_CALL(low_speed_drive, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.low_speed_drive)));
EXPECT_CALL(brake_engaged, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.brake_engaged)));
EXPECT_CALL(low_speed_cut, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.low_speed_cut)));
EXPECT_CALL(blades_enabled, read()).Times(1).WillOnce(testing::Return(toBool(expected_state.blades_enabled)));

manager.tick();

MowerControlState uut_expected_state = manager.getState();
EXPECT_EQ(uut_expected_state.left_motor.throttle_position, expected_state.left_motor.throttle_position);
EXPECT_EQ(uut_expected_state.left_motor.zero_switch, expected_state.left_motor.zero_switch);
EXPECT_EQ(uut_expected_state.right_motor.throttle_position, expected_state.right_motor.throttle_position);
EXPECT_EQ(uut_expected_state.right_motor.zero_switch, expected_state.right_motor.zero_switch);
EXPECT_EQ(uut_expected_state.seat_switch_drive, expected_state.seat_switch_drive);
EXPECT_EQ(uut_expected_state.seat_switch_blade, expected_state.seat_switch_blade);
EXPECT_EQ(uut_expected_state.low_speed_drive, expected_state.low_speed_drive);
EXPECT_EQ(uut_expected_state.brake_engaged, expected_state.brake_engaged);
EXPECT_EQ(uut_expected_state.low_speed_cut, expected_state.low_speed_cut);
EXPECT_EQ(uut_expected_state.blades_enabled, expected_state.blades_enabled);

}
