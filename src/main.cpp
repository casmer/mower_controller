#include <Arduino.h>
#include "sbus/sbus_receiver.hpp"
#include "mower_controller/mower_manual_control_input.hpp"
#include "physical_io/BoardDigitalInput.hpp"
#include "physical_io/BoardAnalogInput.hpp"
#include "mower_controller/mower_control_pin_assignments.hpp"

using namespace cotsbotics::mower_controller;
cotsbotics::SbusReceiver sbus_receiver;

BoardDigitalInput left_motor_zero_switch(MowerControlPinAssignments::Inputs::LEFT_MOTOR_ZERO_SWITCH);
BoardAnalogInput left_motor_throttle(MowerControlPinAssignments::Inputs::LEFT_MOTOR_THROTTLE);
BoardDigitalInput right_motor_zero_switch(MowerControlPinAssignments::Inputs::RIGHT_MOTOR_ZERO_SWITCH);
BoardAnalogInput right_motor_throttle(MowerControlPinAssignments::Inputs::RIGHT_MOTOR_THROTTLE);
BoardDigitalInput seat_switch_drive_controls(MowerControlPinAssignments::Inputs::SEAT_SWITCH_DRIVE_CONTROLS);
BoardDigitalInput seat_switch_blade_controls(MowerControlPinAssignments::Inputs::SEAT_SWITCH_BLADE_CONTROLS);
BoardDigitalInput low_speed_drive(MowerControlPinAssignments::Inputs::LOW_SPEED_DRIVE);
BoardDigitalInput brake_engaged(MowerControlPinAssignments::Inputs::BRAKE_ENGAGED);
BoardDigitalInput low_speed_cut(MowerControlPinAssignments::Inputs::LOW_SPEED_CUT);
BoardDigitalInput blades_enabled(MowerControlPinAssignments::Inputs::BLADES_ENABLED);

cotsbotics::mower_controller:: MowerManualControlInputManager manual_control_input_manager(
left_motor_zero_switch,
left_motor_throttle,
right_motor_zero_switch,
right_motor_throttle,
seat_switch_drive_controls,
seat_switch_blade_controls,
low_speed_drive,
brake_engaged,
low_speed_cut,
blades_enabled
);

static constexpr unsigned long PRINT_DELAY_MS = 40;
millisDelay printDelay;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_receiver.Begin();
  printDelay.start(PRINT_DELAY_MS);

}

void loop () {
  MowerControlState new_state, last_state;
  sbus_receiver.tick();
  manual_control_input_manager.tick();
  
  new_state = manual_control_input_manager.getState();
  if (new_state != last_state || printDelay.justFinished())
  {
    Serial.print("input:");
    Serial.print(new_state.left_motor.throttle_position);
    Serial.print("\t");
    Serial.print(new_state.left_motor.zero_switch_position);
    Serial.print("\t");
    Serial.print(new_state.left_motor.zero_switch ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.right_motor.throttle_position);
    Serial.print("\t");
    Serial.print(new_state.right_motor.zero_switch ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.left_motor.zero_switch_position);
    Serial.print("\t");
    Serial.print(new_state.seat_switch_drive ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.seat_switch_blade ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.low_speed_drive ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.brake_engaged ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.low_speed_cut ? 1 : 0);
    Serial.print("\t");
    Serial.print(new_state.blades_enabled ? 1 : 0);
    Serial.println(":input");
    printDelay.start(PRINT_DELAY_MS);

  }
  last_state = new_state;


  delay(20);
  if (sbus_receiver.no_data() || sbus_receiver.get_failsafe())
  {
    static int print_counter=0;
    print_counter++;


  }
}