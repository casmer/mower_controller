// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>


#include "mower/motor_driver.hpp"
namespace cotsbotics
{
    MotorDriver::MotorDriver(const uint8_t driveOutputPin, const uint8_t driveInputPin,
                const uint8_t zeroSwitchOutputPin, const uint8_t zeroSwitchInputPin) :
                _driveOutputPin(driveOutputPin), _driveInputPin(driveInputPin),
                _zeroSwitchOutputPin(zeroSwitchOutputPin), _zeroSwitchInputPin(zeroSwitchInputPin)
                {

                }

    
}; // namespace cotsbotics

