#pragma once
namespace cotsbotics::mower_controller
{
    class IAnalogInputPort
    {
    public:
        /// @brief Virtual destructor
        virtual ~IAnalogInputPort() = default;

        /// @brief Reads the current analog value from the port.
        virtual float read() const = 0;

    };

}; // cotsbotics::mower_controller