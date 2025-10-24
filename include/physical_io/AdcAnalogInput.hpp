#pragma once

#include "physical_io/IAnalogInputPort.hpp"
#include <Adafruit_ADS1X15.h>

namespace cotsbotics::mower_controller
{
    class AdcAnalogInput : public IAnalogInputPort
    {
    public:

        explicit AdcAnalogInput(int pinNumber, Adafruit_ADS1115& adc);
        /// @brief Virtual destructor
        virtual ~AdcAnalogInput() = default;

        /// @brief Reads the current analog value from the port.
        virtual int read() const override;

        void setup();

        constexpr static uint16_t ANALOG_MAX = 65535U; // 16-bit ADC
        constexpr static uint16_t ANALOG_MIN = 0U; // 16-bit ADC
    private:
        // Pin number for the digital input
        int _pinNumber;
        Adafruit_ADS1115& _adc; 
    };

}; // cotsbotics::mower_controller