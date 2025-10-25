
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/BoardAnalogInput.hpp"


namespace cotsbotics::mower_controller
{
    BoardAnalogInput::BoardAnalogInput(int pin_number)
        : _pin_number(pin_number)
    {}

    int BoardAnalogInput::read() const
    {
        return analogRead(_pin_number);
    }

    void BoardAnalogInput::setup()
    {
        pinMode(_pin_number, INPUT); 
    }

}; // cotsbotics::mower_controller