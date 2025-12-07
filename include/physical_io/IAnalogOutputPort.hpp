// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once
namespace cotsbotics::mower_controller
{
    class IAnalogOutputPort
    {
    public:
        /// @brief Virtual destructor
        virtual ~IAnalogOutputPort() = default;

        /// @brief set the output value of the port
        virtual void write(int value) = 0;

        /// @brief setup the port
        virtual void setup() = 0;
    };

}; // cotsbotics::mower_controller