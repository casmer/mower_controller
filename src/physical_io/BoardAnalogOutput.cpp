
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
            static_cast<uint16_t>(value));
        
    }

    void DacAnalogOutput::setup()
    {
        //nothing to do here
    }

}; // cotsbotics::mower_controller