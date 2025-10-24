
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/AdcAnalogInput.hpp"


namespace cotsbotics::mower_controller
{
    AdcAnalogInput::AdcAnalogInput(int pinNumber, Adafruit_ADS1115& adc)
        : _pinNumber(pinNumber),
            _adc(adc)
    {}
    
uint32_t me_map(uint32_t x, uint32_t in_max, uint32_t out_max)
{
  return (x) * (out_max) / (in_max);
}
    int AdcAnalogInput::read() const
    {
        return static_cast<int>(me_map(_adc.readADC_SingleEnded(_pinNumber), ANALOG_MAX, 4095U));
    }

    void AdcAnalogInput::setup()
    {

        pinMode(_pinNumber, INPUT); 
    }

}; // cotsbotics::mower_controller