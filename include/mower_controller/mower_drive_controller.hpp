// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#ifndef MOWER_DRIVE_CONTROLLER_HPP
#define MOWER_DRIVE_CONTROLLER_HPP


#include "mower_controller.hpp"



class MowerDriveController {
public:
    MowerDriveController();

    // Set left and right motor speeds (-1.0 to 1.0)
    void setTankDrive(double leftSpeed, double rightSpeed);

    // Get current left motor speed
    double getLeftSpeed() const;

    // Get current right motor speed
    double getRightSpeed() const;

private:
    double leftSpeed_;
    double rightSpeed_;

    
};

#endif // MOWER_DRIVE_CONTROLLER_HPP