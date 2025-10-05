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
            static analogPin_t constexpr LEFT_MOTOR_THROTLE = A1;
            static digialPin_t constexpr LEFT_MOTOR_ZERO_SWITCH = D25;
            static analogPin_t constexpr RIGHT_MOTOR_THROTLE = A2;
            static digialPin_t constexpr RIGHT_MOTOR_ZERO_SWITCH = D27;
            static digialPin_t constexpr SEAT_SWITCH_DRIVE_CONTROLS = D23;
            static digialPin_t constexpr SEAT_SWITCH_BLADE_CONTROLS = D31;
            static digialPin_t constexpr LOW_SPEED_DRIVE = D33;
            static digialPin_t constexpr BRAKE_ENGAGED = D35;
            static digialPin_t constexpr LOW_SPEED_CUT = D37;
            static digialPin_t constexpr BLADES_ENABLED = D29;
        };
        class Outputs
        {
        public:
            static dacAnalogPin_t constexpr LEFT_MOTOR_THROTLE = DAC1;
            static digialPin_t constexpr LEFT_MOTOR_ZERO_SWITCH = D24;
            static dacAnalogPin_t constexpr RIGHT_MOTOR_THROTLE = DAC2;
            static digialPin_t constexpr RIGHT_MOTOR_ZERO_SWITCH = D26;
            static digialPin_t constexpr SEAT_SWITCH_DRIVE_CONTROLS = D22;
            static digialPin_t constexpr SEAT_SWITCH_BLADE_CONTROLS = D30;
            static digialPin_t constexpr LOW_SPEED_DRIVE = D32;
            static digialPin_t constexpr BRAKE_ENGAGED = D34;
            static digialPin_t constexpr LOW_SPEED_CUT = D36;
            static digialPin_t constexpr BLADES_ENABLED = D28;
        };
    };
}; // cotsbotics
