#pragma once

class IDigitalOutputPort
{
public:
    /// @brief Virtual destructor
    virtual ~IDigitalOutputPort() = default;

    /// @brief Set the digital output value (true = high, false = low)
    /// @param value The value to set
    virtual void write(bool value) = 0;

    /// @brief For periodic updates if needed
    virtual void tick() = 0;

    /// @brief setup the port
    virtual void setup() = 0;
};