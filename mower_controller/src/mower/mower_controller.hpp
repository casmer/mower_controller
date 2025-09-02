// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef MOWER_CONTROLLER_HPP
#define MOWER_CONTROLLER_HPP
#include <Arduino.h>
#include <pins_arduino.h>

class MowerController {
public:
    MowerController();

    static constexpr uint8_t LeftDriveChannelOutput = A1;
    static constexpr uint8_t LeftDriveChannelManualInput = A2;
    static constexpr uint8_t LeftZeroSwitchChannelOutput = 24;
    static constexpr uint8_t LeftZeroSwitchChannelManualInput = 25;
    static constexpr uint8_t RightDriveSpeedChannel = A3;
    static constexpr uint8_t RightDriveChannelManualInput = A4;
    static constexpr uint8_t RightZeroSwitchChannelOutput = 26;
    static constexpr uint8_t RightZeroSwitchChannelManualInput = 27;
    static constexpr uint8_t SeatSafetySwitchDriveControllerChannelOutput = 22;
    static constexpr uint8_t SeatSafetySwitchDriveControllerChannelInput = 23;
    static constexpr uint8_t SeatSafetySwitchPtoControllerChannelOutput = 30;
    static constexpr uint8_t SeatSafetySwitchPtoControllerChannelInput = 31;
    static constexpr uint8_t LowSpeedCutEnableChannelOutput = 36;
    static constexpr uint8_t LowSpeedCutEnableChannelInput = 37;
    static constexpr uint8_t MowerPtoChannelOutput = 28;
    static constexpr uint8_t MowerPtoChannelManualInput = 29;
    static constexpr uint8_t BrakeEngagedChannelOutput = 34;
    static constexpr uint8_t BrakeEngagedChannelInput = 35;

private:
    double leftSpeed_;
    double rightSpeed_;

    
};

#endif // MOWER_CONTROLLER_HPP