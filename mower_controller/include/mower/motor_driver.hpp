// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef MOTOR_DRIVER_HPP
#define MOTOR_DRIVER_HPP
#include "mower_controller.hpp"
#include <pins_arduino.h>


namespace cotsbotics
{

class MotorDriver {
public:
    MotorDriver(const uint8_t driveOutputPin, const uint8_t driveInputPin,
                const uint8_t zeroSwitchOutputPin, const uint8_t zeroSwitchInputPin);


private:
    uint8_t _driveOutputPin;
    uint8_t _driveInputPin;
    uint8_t _zeroSwitchOutputPin;
    uint8_t _zeroSwitchInputPin;
    
};

} // namespace cotsbotics
#endif // MOTOR_DRIVER_HPP