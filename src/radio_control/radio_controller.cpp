// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "radio_control/radio_controller.hpp"
#include "radio_control/radio_configuration.hpp"

namespace cotsbotics::radio_control
{
    using RC_Channels = RadioConfiguration::Channels;
    using OutputRanges = RadioConfiguration::OutputRanges;

    RadioController::RadioController(cotsbotics::SbusReceiver &sbus_receiver)
        : _sbus_receiver(sbus_receiver)
    {
    }

    void RadioController::tick()
    {
        // Read SBUS data and update control outputs accordingly
        if (!_sbus_receiver.no_data())
        {
            int32_t left_throttle =static_cast<int32_t>(getChannelValueRaw(RC_Channels::LEFT_STICK_Y));
            int32_t right_throttle = static_cast<int32_t>(getChannelValueRaw(RC_Channels::RIGHT_STICK_Y));
            _radio_control_state.left_throttle_position =  _throttle_converter.convert(left_throttle);
            _radio_control_state.right_throttle_position = _throttle_converter.convert(right_throttle);
            _radio_control_state.zero_switch =  getChannelSwitchState(RC_Channels::ZERO_SWITCH);
            _radio_control_state.seat_switch_blade = getChannelSwitchState(RC_Channels::SEAT_SWITCH);
            _radio_control_state.seat_switch_drive = getChannelSwitchState(RC_Channels::SEAT_SWITCH);
            _radio_control_state.blades_enabled = getChannelSwitchState(RC_Channels::BLADE_ENABLE) ;
            _radio_control_state.low_speed_drive = getChannelSwitchState(RC_Channels::LOW_SPEED_DRIVE);
            _radio_control_state.low_speed_cut = getChannelSwitchState(RC_Channels::LOW_SPEED_CUT);

        }
    }

    bool RadioController::is_receiving_signal()
    {
        return (!_sbus_receiver.no_data()) || _sbus_receiver.get_failsafe();
    }

    uint16_t RadioController::getChannelValueRaw(int8_t channel)
    {
        return _sbus_receiver.get_channel(channel);
    }

    bool RadioController::getChannelValueBool(int8_t channel)
    {
        return _sbus_receiver.get_channel(channel) > RadioConfiguration::ChannelRanges::NEUTRAL_INPUT;
    }   
    RadioSwitch RadioController::getChannelSwitchState(int8_t channel)
    {
        return fromBool(getChannelValueBool(channel));
    }

}; // namespace cotsbotics::radio_control