// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

class IDigitalInputPort
{
public:
    /// @brief Virtual destructor
    virtual ~IDigitalInputPort() = default;

    /// @brief Get the raw digital input value without debouncing (true = high, false = low)
    virtual bool read_raw() const = 0;

    /// @brief Get the debounced digital input value (true = high, false = low)
    virtual bool read() const = 0;

    /// @brief Setup the port
    virtual void setup() = 0;

    /// @brief Update debounce state (call periodically)
    virtual void tick() = 0;
};