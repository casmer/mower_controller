// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "radio_control/radio_control_state.hpp"
#include "radio_control/radio_configuration.hpp"
#include "radio_control/throttle_converter.hpp"
#include "sbus/sbus_receiver.hpp"

namespace cotsbotics::radio_control
{
    class RadioController
    {
    public:
        RadioController(cotsbotics::SbusReceiver &sbus_receiver);

        /// @brief Run periodic tasks for the RC control
        void tick();

        RadioControlState const &getState() const noexcept
        {
            return _radio_control_state;
        }

    private:
        cotsbotics::SbusReceiver &_sbus_receiver;
        RadioControlState _radio_control_state{};

        void stopAllMotors();

        bool is_receiving_signal();
        uint16_t getChannelValueRaw(int8_t channel);
        bool getChannelValueBool(int8_t channel);
        RadioSwitch getChannelSwitchState(int8_t channel);
        ThrottleConverter _throttle_converter{
            RadioConfiguration::ChannelRanges::MIN_INPUT,
            RadioConfiguration::ChannelRanges::MAX_INPUT,
            RadioConfiguration::ChannelRanges::DEADBAND,
            RadioConfiguration::OutputRanges::MIN_OUTPUT,
            RadioConfiguration::OutputRanges::MAX_OUTPUT
        };
    };

}; // namespace cotsbotics::mower_controller