// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef MOTOR_DRIVER_HPP
#define MOTOR_DRIVER_HPP

#include "physical_io/IAnalogOutputPort.hpp"
#include "physical_io/IDigitalOutputPort.hpp"

namespace cotsbotics::mower_controller
{

class MotorDriver {
public:
    MotorDriver(IAnalogOutputPort& driveOutputPin,
                IDigitalOutputPort& zeroSwitchOutputPin);

    /// @brief Run periodic tasks for the motor driver
    void tick();

    void setSpeed(double speed);
    double getSetSpeed() const;
    double getCurrentSpeed() const;


private:
    IAnalogOutputPort& _driveOutputPin;
    IDigitalOutputPort& _zeroSwitchOutputPin;

    double _setSpeed{0.0};
    double _currentSpeed{0.0};
    
};

} // namespace cotsbotics
#endif // MOTOR_DRIVER_HPP