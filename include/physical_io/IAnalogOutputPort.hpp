#pragma once
namespace cotsbotics::mower_controller
{
    class IAnalogOutputPort
    {
    public:
        /// @brief Virtual destructor
        virtual ~IAnalogOutputPort() = default;

        /// @brief Optionally, set the port to a specific value (if supported).
        virtual void write(float value) = 0;

        /// @brief For periodic updates if needed
        virtual void tick() = 0;
    };

}; // cotsbotics::mower_controller