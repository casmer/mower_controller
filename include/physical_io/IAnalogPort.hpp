#pragma once
namespace cotsbotics::mower_controller
{
    class IAnalogPort
    {
    public:
        /// @brief Virtual destructor
        virtual ~IAnalogPort() = default;

        /// @brief Reads the current analog value from the port.
        virtual float read() const = 0;

        /// @brief Optionally, set the port to a specific value (if supported).
        virtual void write(float value) = 0;

        /// @brief For periodic updates if needed
        virtual void tick() = 0;
    };

}; // cotsbotics::mower_controller