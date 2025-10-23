#include <Arduino.h>
#include <Adafruit_MCP4728.h>
#include "sbus/sbus_receiver.hpp"
#include "mower_controller/mower_manual_control_input.hpp"
#include "mower_controller/mower_control_output.hpp"
#include "physical_io/BoardDigitalInput.hpp"
#include "physical_io/BoardDigitalOutput.hpp"
#include "physical_io/BoardAnalogInput.hpp"
#include "physical_io/DacAnalogOutput.hpp"
#include "mower_controller/mower_control_pin_assignments.hpp"

using namespace cotsbotics::mower_controller;
cotsbotics::SbusReceiver sbus_receiver;
Adafruit_MCP4728 mcp;

BoardDigitalInput in_left_motor_zero_switch(MowerControlPinAssignments::Inputs::LEFT_MOTOR_ZERO_SWITCH);
BoardAnalogInput in_left_motor_throttle(MowerControlPinAssignments::Inputs::LEFT_MOTOR_THROTTLE);
BoardDigitalInput in_right_motor_zero_switch(MowerControlPinAssignments::Inputs::RIGHT_MOTOR_ZERO_SWITCH);
BoardAnalogInput in_right_motor_throttle(MowerControlPinAssignments::Inputs::RIGHT_MOTOR_THROTTLE);
BoardDigitalInput in_seat_switch_drive_controls(MowerControlPinAssignments::Inputs::SEAT_SWITCH_DRIVE_CONTROLS);
BoardDigitalInput in_seat_switch_blade_controls(MowerControlPinAssignments::Inputs::SEAT_SWITCH_BLADE_CONTROLS);
BoardDigitalInput in_low_speed_drive(MowerControlPinAssignments::Inputs::LOW_SPEED_DRIVE);
BoardDigitalInput in_brake_engaged(MowerControlPinAssignments::Inputs::BRAKE_ENGAGED);
BoardDigitalInput in_low_speed_cut(MowerControlPinAssignments::Inputs::LOW_SPEED_CUT);
BoardDigitalInput in_blades_enabled(MowerControlPinAssignments::Inputs::BLADES_ENABLED);


BoardDigitalOutput out_left_motor_zero_switch(MowerControlPinAssignments::Outputs::LEFT_MOTOR_ZERO_SWITCH);
DacAnalogOutput out_left_motor_throttle(MowerControlPinAssignments::Outputs::LEFT_MOTOR_THROTTLE, mcp);
BoardDigitalOutput out_right_motor_zero_switch(MowerControlPinAssignments::Outputs::RIGHT_MOTOR_ZERO_SWITCH);
DacAnalogOutput out_right_motor_throttle(MowerControlPinAssignments::Outputs::RIGHT_MOTOR_THROTTLE, mcp);
BoardDigitalOutput out_seat_switch_drive_controls(MowerControlPinAssignments::Outputs::SEAT_SWITCH_DRIVE_CONTROLS);
BoardDigitalOutput out_seat_switch_blade_controls(MowerControlPinAssignments::Outputs::SEAT_SWITCH_BLADE_CONTROLS);
BoardDigitalOutput out_low_speed_drive(MowerControlPinAssignments::Outputs::LOW_SPEED_DRIVE);
BoardDigitalOutput out_brake_engaged(MowerControlPinAssignments::Outputs::BRAKE_ENGAGED);
BoardDigitalOutput out_low_speed_cut(MowerControlPinAssignments::Outputs::LOW_SPEED_CUT);
BoardDigitalOutput out_blades_enabled(MowerControlPinAssignments::Outputs::BLADES_ENABLED);

cotsbotics::mower_controller:: MowerManualControlInputManager manual_control_input_manager(
in_left_motor_zero_switch,
in_left_motor_throttle,
in_right_motor_zero_switch,
in_right_motor_throttle,
in_seat_switch_drive_controls,
in_seat_switch_blade_controls,
in_low_speed_drive,
in_brake_engaged,
in_low_speed_cut,
in_blades_enabled
);


cotsbotics::mower_controller:: MowerControlOutputManager control_output_manager(
out_left_motor_zero_switch,
out_left_motor_throttle,
out_right_motor_zero_switch,
out_right_motor_throttle,
out_seat_switch_drive_controls,
out_seat_switch_blade_controls,
out_low_speed_drive,
out_brake_engaged,
out_low_speed_cut,
out_blades_enabled
);
static constexpr unsigned long PRINT_DELAY_MS = 40;
millisDelay inputPrintDelay;
millisDelay outputPrintDelay;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_receiver.Begin();

  
  // Try to initialize!
  if (!mcp.begin()) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }

  inputPrintDelay.start(PRINT_DELAY_MS);
  outputPrintDelay.start(PRINT_DELAY_MS);

}
void printState(MowerControlState const &state, bool is_input)
{
    if (is_input)
    {
      Serial.print("input:");
    }
    else
    {
      Serial.print("output:");
    }
    Serial.print(state.left_motor.throttle_position);
    Serial.print("\t");
    Serial.print(state.left_motor.zero_switch_position);
    Serial.print("\t");
    Serial.print(static_cast<int>(state.left_motor.zero_switch));
    Serial.print("\t");
    Serial.print(state.right_motor.throttle_position);
    Serial.print("\t");
    Serial.print(static_cast<int>(state.right_motor.zero_switch));
    Serial.print("\t");
    Serial.print(state.left_motor.zero_switch_position);
    Serial.print("\t");
    Serial.print(static_cast<int>(state.seat_switch_drive));
    Serial.print("\t");
    Serial.print(static_cast<int>(state.seat_switch_blade));
    Serial.print("\t");
    Serial.print(static_cast<int>(state.low_speed_drive));
    Serial.print("\t");
    Serial.print(static_cast<int>(state.brake_engaged));
    Serial.print("\t");
    Serial.print(static_cast<int>(state.low_speed_cut));
    Serial.print("\t");
    Serial.print(static_cast<int>(state.blades_enabled));
    if (is_input)
    {
          Serial.println(":input");
    }
    else
    {
          Serial.println(":output");
    }
}


void loop () {
  MowerControlState new_state, last_state;

  MowerControlState new_output_state, last_output_state;
  sbus_receiver.tick();
  manual_control_input_manager.tick();
  control_output_manager.tick();

  new_state = manual_control_input_manager.getState();
  if (new_state != last_state || inputPrintDelay.justFinished())
  {
    printState(new_state, true);
    inputPrintDelay.start(PRINT_DELAY_MS);

  }
  last_state = new_state;
  
  new_output_state = control_output_manager.getState();
  if (new_output_state != last_output_state || outputPrintDelay.justFinished())
  {
    printState(new_output_state, false);
    outputPrintDelay.start(PRINT_DELAY_MS);

  }
  last_output_state = new_output_state;


  delay(20);
  if (sbus_receiver.no_data() || sbus_receiver.get_failsafe())
  {
    static int print_counter=0;
    print_counter++;


  }
}