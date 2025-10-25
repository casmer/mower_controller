#pragma once

#include "pin_definitions.hpp"
using namespace cotsbotics::mower_controller;

namespace cotsbotics::mower_controller
{
    class MowerControlPinAssignments
    {
    public:
        class Inputs
        {
        public:
            static analogPin_t constexpr LEFT_MOTOR_THROTTLE = ADC1;
            static digitalPin_t constexpr LEFT_MOTOR_ZERO_SWITCH = D25;
            static analogPin_t constexpr RIGHT_MOTOR_THROTTLE = ADC0;
            static digitalPin_t constexpr RIGHT_MOTOR_ZERO_SWITCH = D27;
            static digitalPin_t constexpr SEAT_SWITCH_DRIVE_CONTROLS = D23;
            static digitalPin_t constexpr SEAT_SWITCH_BLADE_CONTROLS = D31;
            static digitalPin_t constexpr LOW_SPEED_DRIVE = D33;
            static digitalPin_t constexpr BRAKE_ENGAGED = D35;
            static digitalPin_t constexpr LOW_SPEED_CUT = D37;
            static digitalPin_t constexpr BLADES_ENABLED = D29;
        };
        class Outputs
        {
        public:
            static analogPin_t constexpr LEFT_MOTOR_THROTTLE = MCP4728_CHANNEL_A;
            static digitalPin_t constexpr LEFT_MOTOR_ZERO_SWITCH = D24;
            static analogPin_t constexpr RIGHT_MOTOR_THROTTLE = MCP4728_CHANNEL_B;
            static digitalPin_t constexpr RIGHT_MOTOR_ZERO_SWITCH = D26;
            static digitalPin_t constexpr SEAT_SWITCH_DRIVE_CONTROLS = D22;
            static digitalPin_t constexpr SEAT_SWITCH_BLADE_CONTROLS = D30;
            static digitalPin_t constexpr LOW_SPEED_DRIVE = D32;
            static digitalPin_t constexpr BRAKE_ENGAGED = D34;
            static digitalPin_t constexpr LOW_SPEED_CUT = D36;
            static digitalPin_t constexpr BLADES_ENABLED = D28;
        };
    };
}; // cotsbotics
