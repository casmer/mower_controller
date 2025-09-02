// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>


#include "mower/motor_driver.hpp"

    MotorDriver(const uint8_t driveOutputPin, const uint8_t driveInputPin,
                const uint8_t zeroSwitchOutputPin, const uint8_t zeroSwitchInputPin);


    uint8_t _driveOutputPin;
    uint8_t _driveInputPin;
    uint8_t _zeroSwitchOutputPin;
    uint8_t _zeroSwitchInputPin;
    
};

