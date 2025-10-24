
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/BoardDigitalInput.hpp"


namespace cotsbotics::mower_controller
{
    BoardDigitalInput::BoardDigitalInput(int pinNumber)
        : _pinNumber(pinNumber)
    {}

    bool BoardDigitalInput::read() const
    {
        return (digitalRead(_pinNumber)==HIGH); 
    }

    void BoardDigitalInput::setup()
    {
        pinMode(_pinNumber, INPUT); 
    }

}; // cotsbotics::mower_controller