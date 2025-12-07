// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

class IDigitalInputPort
{
public:
    /// @brief Virtual destructor
    virtual ~IDigitalInputPort() = default;

    /// @brief Get the current digital input value (true = high, false = low)
    virtual bool read() const = 0;

    /// @brief setup the port
    virtual void setup() = 0;
};