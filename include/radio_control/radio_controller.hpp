#pragma once

#include "mower_controller/mower_control_output.hpp"
#include "mower_controller/mower_manual_control_input.hpp"
#include "radio_control/radio_configuration.hpp"
#include "radio_control/throttle_converter.hpp"
#include "sbus/sbus_receiver.hpp"

namespace cotsbotics::radio_control
{
    class RadioController
    {
    public:
        RadioController(cotsbotics::SbusReceiver &sbus_receiver,
                        MowerControlOutputManager &control_output_manager,
                        MowerManualControlInputManager &manual_control_input_manager);

        /// @brief Run periodic tasks for the RC control
        void tick();

    private:
        cotsbotics::SbusReceiver &_sbus_receiver;
        MowerControlOutputManager &_control_output_manager;
        MowerManualControlInputManager &_manual_control_input_manager;

        void stopAllMotors();

        bool is_receiving_signal();
        uint16_t getChannelValueRaw(int8_t channel);
        bool getChannelValueBool(int8_t channel);
        MowerSwitch getChannelSwitchState(int8_t channel);
        ThrottleConverter _throttle_converter{
            RadioConfiguration::ChannelRanges::MIN_INPUT,
            RadioConfiguration::ChannelRanges::MAX_INPUT,
            RadioConfiguration::ChannelRanges::DEADBAND,
            RadioConfiguration::OutputRanges::MIN_OUTPUT,
            RadioConfiguration::OutputRanges::MAX_OUTPUT
        };
    };

}; // namespace cotsbotics::mower_controller