
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/BoardAnalogInput.hpp"


namespace cotsbotics::mower_controller
{
    BoardAnalogInput::BoardAnalogInput(int pinNumber)
        : _pinNumber(pinNumber)
    {}

    int BoardAnalogInput::read() const
    {
        return analogRead(_pinNumber);
    }

    void BoardAnalogInput::setup()
    {
        pinMode(_pinNumber, OUTPUT); 
    }

}; // cotsbotics::mower_controller