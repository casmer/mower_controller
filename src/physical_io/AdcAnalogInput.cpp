// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/AdcAnalogInput.hpp"


namespace cotsbotics::mower_controller
{
    AdcAnalogInput::AdcAnalogInput(int pin_number, AdcManager& adc_manager)
        : _pin_number(pin_number),
            _adc_manager(adc_manager)
    {}
    
int me_map(int x, int in_max, int out_max)
{
  return (x) * (out_max) / (in_max);
}
    int AdcAnalogInput::read() const
    {
        // return _adc_manager.readChannel(_pin_number);
        return static_cast<int>(map(_adc_manager.readChannel(_pin_number), 0, 26666,0, 4095));
    }

    void AdcAnalogInput::setup()
    {
        // No setup required for ADC channel
    }

}; // cotsbotics::mower_controller