#include <gmock/gmock.h>
#include <mock/MockAnalogPort.hpp>
#include <mock/MockDigitalPort.hpp>
#include "mower_controller/mower_manual_control_input.hpp"

using namespace cotsbotics::mower_controller;
TEST(mower_manual_control_input, tick)
{
MockDigitalPort left_motor_zero_switch;
MockAnalogPort left_motor_throtle;
MockDigitalPort right_motor_zero_switch;
MockAnalogPort right_motor_throtle;
MockDigitalPort seat_switch_drive_controls;
MockDigitalPort seat_switch_blade_controls;
MockDigitalPort low_speed_drive;
MockDigitalPort brake_engaged;
MockDigitalPort low_speed_cut;
MockDigitalPort blades_enabled;

MowerControlState expected_state;
expected_state.left_motor.throtle_position = 0.5f;
expected_state.left_motor.zero_switch = true;
expected_state.right_motor.throtle_position = -0.5f;    
expected_state.right_motor.zero_switch = false;
expected_state.seat_switch_drive = true;
expected_state.seat_switch_blade = false;
expected_state.low_speed_drive = true;
expected_state.brake_engaged = false;
expected_state.low_speed_cut = true;
expected_state.blades_enabled = false;

MowerManualControlInputManager manager(
    left_motor_zero_switch,
    left_motor_throtle,
    right_motor_zero_switch,
    right_motor_throtle,
    seat_switch_drive_controls,
    seat_switch_blade_controls,
    low_speed_drive,
    brake_engaged,
    low_speed_cut,
    blades_enabled);

EXPECT_CALL(left_motor_zero_switch, tick()).Times(1);
EXPECT_CALL(left_motor_throtle, tick()).Times(1);
EXPECT_CALL(right_motor_zero_switch, tick()).Times(1);
EXPECT_CALL(right_motor_throtle, tick()).Times(1);
EXPECT_CALL(seat_switch_drive_controls, tick()).Times(1);
EXPECT_CALL(seat_switch_blade_controls, tick()).Times(1);
EXPECT_CALL(low_speed_drive, tick()).Times(1);
EXPECT_CALL(brake_engaged, tick()).Times(1);
EXPECT_CALL(low_speed_cut, tick()).Times(1);
EXPECT_CALL(blades_enabled, tick()).Times(1);

EXPECT_CALL(left_motor_zero_switch, read()).Times(1).WillOnce(testing::Return(expected_state.left_motor.zero_switch));
EXPECT_CALL(left_motor_throtle, read()).Times(1).WillOnce(testing::Return(expected_state.left_motor.throtle_position));
EXPECT_CALL(right_motor_zero_switch, read()).Times(1).WillOnce(testing::Return(expected_state.right_motor.zero_switch));
EXPECT_CALL(right_motor_throtle, read()).Times(1).WillOnce(testing::Return(expected_state.right_motor.throtle_position));
EXPECT_CALL(seat_switch_drive_controls, read()).Times(1).WillOnce(testing::Return(expected_state.seat_switch_drive));
EXPECT_CALL(seat_switch_blade_controls, read()).Times(1).WillOnce(testing::Return(expected_state.seat_switch_blade));
EXPECT_CALL(low_speed_drive, read()).Times(1).WillOnce(testing::Return(expected_state.low_speed_drive));
EXPECT_CALL(brake_engaged, read()).Times(1).WillOnce(testing::Return(expected_state.brake_engaged));
EXPECT_CALL(low_speed_cut, read()).Times(1).WillOnce(testing::Return(expected_state.low_speed_cut));
EXPECT_CALL(blades_enabled, read()).Times(1).WillOnce(testing::Return(expected_state.blades_enabled));

manager.tick();

MowerControlState uut_expected_state = manager.getState();
EXPECT_EQ(uut_expected_state.left_motor.throtle_position, expected_state.left_motor.throtle_position);
EXPECT_EQ(uut_expected_state.left_motor.zero_switch, expected_state.left_motor.zero_switch);
EXPECT_EQ(uut_expected_state.right_motor.throtle_position, expected_state.right_motor.throtle_position);
EXPECT_EQ(uut_expected_state.right_motor.zero_switch, expected_state.right_motor.zero_switch);
EXPECT_EQ(uut_expected_state.seat_switch_drive, expected_state.seat_switch_drive);
EXPECT_EQ(uut_expected_state.seat_switch_blade, expected_state.seat_switch_blade);
EXPECT_EQ(uut_expected_state.low_speed_drive, expected_state.low_speed_drive);
EXPECT_EQ(uut_expected_state.brake_engaged, expected_state.brake_engaged);
EXPECT_EQ(uut_expected_state.low_speed_cut, expected_state.low_speed_cut);
EXPECT_EQ(uut_expected_state.blades_enabled, expected_state.blades_enabled);

}
// TEST_F(...)

#if defined(ARDUINO)
#include <Arduino.h>

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleMock();
}

void loop()
{
  // Run tests
  if (RUN_ALL_TESTS())
  ;

  // sleep for 1 sec
  delay(1000);
}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleMock(&argc, argv);

    if (RUN_ALL_TESTS())
    ;

    // Always return zero-code and allow PlatformIO to parse results
    return 0;
}
#endif