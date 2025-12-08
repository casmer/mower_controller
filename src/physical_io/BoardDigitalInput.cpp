// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/BoardDigitalInput.hpp"


namespace cotsbotics::mower_controller
{
    BoardDigitalInput::BoardDigitalInput(int pin_number)
        : _pin_number(pin_number)
    {}

    bool BoardDigitalInput::read_raw() const
    {
        return (digitalRead(_pin_number)==HIGH); 
    }

    bool BoardDigitalInput::read() const
    {
        return _debounced_state;
    }

    void BoardDigitalInput::setup()
    {
        pinMode(_pin_number, INPUT_PULLUP); // Enable internal pull-up resistor
        _current_state = read_raw();
        _debounced_state = _current_state;
        _stable_ticks = 0;
    }

    void BoardDigitalInput::tick()
    {
        bool new_state = read_raw();

        // If state has changed, reset the debounce counter
        if (new_state != _current_state)
        {
            _current_state = new_state;
            _stable_ticks = 0;
        }
        else
        {
            // State is stable, increment tick counter
            _stable_ticks++;

            // If state has been stable for enough ticks, update debounced state
            if (_stable_ticks >= _debounce_delay_ticks)
            {
                _debounced_state = _current_state;
            }
        }
    }

    void BoardDigitalInput::set_debounce_delay_ticks(uint32_t debounce_ticks)
    {
        _debounce_delay_ticks = debounce_ticks;
    }

}; // cotsbotics::mower_controller