// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/BoardDigitalOutput.hpp"


namespace cotsbotics::mower_controller
{
    BoardDigitalOutput::BoardDigitalOutput(int pinNumber)
        : _pinNumber(pinNumber)
    {}

    void BoardDigitalOutput::write(bool value)
    {
        digitalWrite(_pinNumber, value ? HIGH : LOW); 
    }

    void BoardDigitalOutput::setup()
    {
        pinMode(_pinNumber, OUTPUT); 
    }

}; // cotsbotics::mower_controller