#pragma once

// Demo for configuring the Vref of the MCP4728 4-Channel 12-bit I2C DAC
#include <Adafruit_MCP4728.h>


#include "physical_io/IAnalogOutputPort.hpp"

namespace cotsbotics::mower_controller
{
    class DacAnalogOutput : public IAnalogOutputPort
    {
    public:

        explicit DacAnalogOutput(MCP4728_channel_t pinNumber, Adafruit_MCP4728& mcp);
        /// @brief Virtual destructor
        virtual ~DacAnalogOutput() = default;

        /// @brief Set the digital output value (true = high, false = low)
        /// @param value The value to set
        virtual void write(int value) override;

        void setup();
    private:
        // Pin number for the digital input
        MCP4728_channel_t _channel;
        Adafruit_MCP4728& _mcp;

    };

}; // cotsbotics::mower_controller