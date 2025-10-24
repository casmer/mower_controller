
#include <Arduino.h> // or appropriate header for your platform
#include "physical_io/DacAnalogOutput.hpp"


namespace cotsbotics::mower_controller
{
    DacAnalogOutput::DacAnalogOutput(MCP4728_channel_t channel, Adafruit_MCP4728& mcp)
            : _channel(channel), _mcp(mcp)
        {}

    void DacAnalogOutput::write(int value)
    {
        _mcp.setChannelValue(
            _channel,
            constrain(static_cast<uint16_t>(value), MIN_DAC_VALUE, MAX_DAC_VALUE),
            MCP4728_VREF_VDD,
            MCP4728_GAIN_2X);
        
    }

    void DacAnalogOutput::setup()
    {
        uint16_t current_value = _mcp.getChannelValue(_channel);
        if (current_value != (MIN_DAC_VALUE + MAX_DAC_VALUE) / 2)
        {
            _mcp.setChannelValue(
                _channel,
                (MIN_DAC_VALUE + MAX_DAC_VALUE) / 2,
            MCP4728_VREF_VDD,
            MCP4728_GAIN_2X);
            _mcp.saveToEEPROM();
        }
    }

}; // cotsbotics::mower_controller