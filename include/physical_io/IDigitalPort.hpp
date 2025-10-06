#pragma once

class IDigitalPort
{
public:
    /// @brief Virtual destructor
    virtual ~IDigitalPort() = default;

    /// @brief Set the digital output value (true = high, false = low)
    /// @param value The value to set
    virtual void write(bool value) = 0;

    /// @brief Get the current digital input value (true = high, false = low)
    virtual bool read() const = 0;

    /// @brief For periodic updates if needed
    virtual void tick() = 0;
};