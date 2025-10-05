#pragma once

class IDigitalPort
{
public:
    /// @brief Virtual destructor
    virtual ~IDigitalPort() = default;

    /// @brief Set the digital output value (true = high, false = low)
    /// @param value The value to set
    virtual void setValue(bool value) = 0;

    /// @brief Get the current digital input value (true = high, false = low)
    virtual bool getValue() const = 0;

    /// @brief Optionally, configure the port as input or output
    /// @param output true to set as output, false for input
    virtual void setDirection(bool output) = 0;

    /// @brief For periodic updates if needed
    virtual void tick() = 0;
};