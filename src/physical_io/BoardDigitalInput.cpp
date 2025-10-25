
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/BoardDigitalInput.hpp"


namespace cotsbotics::mower_controller
{
    BoardDigitalInput::BoardDigitalInput(int pin_number)
        : _pin_number(pin_number)
    {}

    bool BoardDigitalInput::read() const
    {
        return (digitalRead(_pin_number)==HIGH); 
    }

    void BoardDigitalInput::setup()
    {
        pinMode(_pin_number, INPUT); 
    }

}; // cotsbotics::mower_controller