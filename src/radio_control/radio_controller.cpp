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
        _sbus_receiver.tick();
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
            _radio_control_state.control_mode = getChannelSwitchState(RC_Channels::CONTROL_MODE);
        }
        else
        {
            int32_t left_throttle =static_cast<int32_t>(RadioConfiguration::ChannelRanges::NEUTRAL_INPUT);
            int32_t right_throttle = static_cast<int32_t>(RadioConfiguration::ChannelRanges::NEUTRAL_INPUT);
            _radio_control_state.left_throttle_position =  _throttle_converter.convert(left_throttle);
            _radio_control_state.right_throttle_position = _throttle_converter.convert(right_throttle);
            _radio_control_state.zero_switch =  RadioSwitch::RS_LOW;
            _radio_control_state.seat_switch_blade = RadioSwitch::RS_LOW;
            _radio_control_state.seat_switch_drive = RadioSwitch::RS_LOW;
            _radio_control_state.blades_enabled = RadioSwitch::RS_LOW;
            _radio_control_state.low_speed_drive = RadioSwitch::RS_LOW;
            _radio_control_state.low_speed_cut = RadioSwitch::RS_LOW;
            _radio_control_state.control_mode = RadioSwitch::RS_LOW;
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

    RadioSwitch RadioController::getChannelSwitchState(int8_t channel)
    {
        uint16_t value = getChannelValueRaw(channel);
        if (value <= RadioConfiguration::ChannelRanges::TRI_STATE_LOW_INPUT)
        {
            return RadioSwitch::RS_LOW;
        }
        else if (value >= RadioConfiguration::ChannelRanges::TRI_STATE_HIGH_INPUT)
        {
            return RadioSwitch::RS_HIGH;
        }
        else
        {
            return RadioSwitch::RS_MIDDLE;
        }
    }

}; // namespace cotsbotics::radio_control