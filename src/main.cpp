// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include <Arduino.h>
#include <Adafruit_MCP4728.h>
#include "sbus/sbus_receiver.hpp"
#include "mower_controller/mower_manual_control_input.hpp"
#include "mower_controller/mower_control_output.hpp"
#include "physical_io/BoardDigitalInput.hpp"
#include "physical_io/BoardDigitalOutput.hpp"
#include "physical_io/AdcAnalogInput.hpp"
#include "physical_io/DacAnalogOutput.hpp"
#include "mower_controller/mower_control_pin_assignments.hpp"
#include "radio_control/radio_controller.hpp"
#include "physical_io/AdcManager.hpp"
#include  "millisDelay.h"
using namespace cotsbotics::mower_controller;
cotsbotics::SbusReceiver sbus_receiver;
Adafruit_MCP4728 mcp;
Adafruit_ADS1115  ads;
AdcManager adc_manager(ads);

BoardDigitalInput in_left_motor_zero_switch(MowerControlPinAssignments::Inputs::LEFT_MOTOR_ZERO_SWITCH);
AdcAnalogInput in_left_motor_throttle(MowerControlPinAssignments::Inputs::LEFT_MOTOR_THROTTLE, adc_manager);
BoardDigitalInput in_right_motor_zero_switch(MowerControlPinAssignments::Inputs::RIGHT_MOTOR_ZERO_SWITCH);
AdcAnalogInput in_right_motor_throttle(MowerControlPinAssignments::Inputs::RIGHT_MOTOR_THROTTLE, adc_manager);
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

cotsbotics::radio_control::RadioController radio_controller(
    sbus_receiver
);
static constexpr unsigned long PRINT_DELAY_MS = 100;
millisDelay printDelay;

millisDelay changeValueDelayTimer;
MowerControlState last_state, last_output_state;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_receiver.Begin();

  int retrys = 10;
  while (retrys-- > 0) {
    if (mcp.begin()) {
      break;
    }
    Serial.println("MCP4728 not found, retrying...");
    delay(100);
  }
  if (retrys <= 0) {
    Serial.println("Failed to find MCP4728 chip");
    while (1) {
      delay(10);
    }
  }
  ads.setGain(GAIN_TWOTHIRDS);
  ads.setDataRate(RATE_ADS1015_3300SPS);
  retrys = 10;
  while (retrys-- > 0) {
    if (ads.begin()) {
      break;
    }
    Serial.println("ADS not found, retrying...");
    delay(100);
  } 
  if (retrys <= 0) {
    Serial.println("Failed to find ADS chip");
    while (1) {
      delay(10);
    }
  }

  // ads.setVoltageRange_mV(ADS1115_RANGE_6144);
  // ads.setCompareChannels(ADS1115_COMP_0_GND);
  // ads.setMeasureMode(ADS1115_CONTINUOUS); 
 last_output_state.left_motor.zero_switch_position = -1;
 last_output_state.right_motor.zero_switch_position = -1;
  control_output_manager.setup();
  manual_control_input_manager.setup();
  adc_manager.setup();
  printDelay.start(PRINT_DELAY_MS);
  changeValueDelayTimer.start(600);

}

void printSbusState(cotsbotics::SbusReceiver &data)
{
      /* Display the received data */
      Serial.print("sbsus:");
      for (int8_t i = 0; i < data.NUM_CH; i++) {
          Serial.print(data.get_channel(i));
          Serial.print("\t");
      }
      /* Display lost frames and failsafe data */                   
      Serial.print(data.get_lost_frame()?"LF":"--");
      Serial.print("\t");
      Serial.print(data.get_failsafe()?"FS":"--");
      Serial.println(":sb");
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
    Serial.print(state.left_motor.zero_switch_position);
    Serial.print("\t");
    Serial.print(static_cast<int>(state.right_motor.zero_switch));
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


  
  sbus_receiver.tick();
  manual_control_input_manager.tick();
  control_output_manager.tick();
 adc_manager.tick();
 radio_controller.tick();

  if (printDelay.justFinished())
  {
    printState(manual_control_input_manager.getState(), true);
  
    printState(control_output_manager.getState(), false);
    printSbusState(sbus_receiver);
    printDelay.start(PRINT_DELAY_MS);
  }

  // static int throttle_position = 4095;
  // if (changeValueDelayTimer.justFinished())
  // {
  //   // Toggle some outputs for testing
  //   static bool toggle = false;
  //   toggle = !toggle;
  //   control_output_manager.getState().blades_enabled =fromBool(toggle);
  //   control_output_manager.getState().low_speed_cut = fromBool(toggle);
  //   control_output_manager.getState().left_motor.throttle_position = throttle_position;;
  //   control_output_manager.getState().right_motor.throttle_position = 4095 - throttle_position;

  //   if (throttle_position >= 4095)
  //   {
  //     throttle_position = 0;
  //   }
  //   else
  //   {
  //     throttle_position+= (4096/4);
  //   }
  //   changeValueDelayTimer.start(2000);
  //   Serial.print("Throttle: ");
  //   Serial.print(throttle_position);
  //   Serial.print(" toggle: ");
  //   Serial.println(toggle);

  // }

  delayMicroseconds(10);

}