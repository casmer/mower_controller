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

    /// @brief Run periodic tasks for the motor driver
    void tick();
    /// @brief Set manual mode
    /// @param manualMode Manual mode flag (true for manual, false for automatic)
    void setManualMode(bool manualMode);
    /// @brief Get current manual mode status
    /// @return true if in manual mode, false if in automatic mode
    bool getManualMode() const;


private:
    uint8_t _driveOutputPin;
    uint8_t _driveInputPin;
    uint8_t _zeroSwitchOutputPin;
    uint8_t _zeroSwitchInputPin;
    bool _manualMode{true};
    
};

} // namespace cotsbotics
#endif // MOTOR_DRIVER_HPP