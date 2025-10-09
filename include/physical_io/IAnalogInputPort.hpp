#pragma once
namespace cotsbotics::mower_controller
{
    class IAnalogInputPort
    {
    public:
        /// @brief Virtual destructor
        virtual ~IAnalogInputPort() = default;

        /// @brief Reads the current analog value from the port.
        virtual int read() const = 0;
        
        /// @brief setup the port
        virtual void setup() = 0;
    };

}; // cotsbotics::mower_controller