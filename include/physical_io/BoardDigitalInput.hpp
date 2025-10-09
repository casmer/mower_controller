#pragma once

#include "physical_io/IDigitalInputPort.hpp"

namespace cotsbotics::mower_controller
{
    class BoardDigitalInput : public IDigitalInputPort
    {
    public:

        explicit BoardDigitalInput(int pinNumber);
        /// @brief Virtual destructor
        virtual ~BoardDigitalInput() = default;

        /// @brief Reads the current digital value from the port.
        virtual bool read() const override;

        void setup();
    private:
        // Pin number for the digital input
        int _pinNumber;

    };

}; // cotsbotics::mower_controller