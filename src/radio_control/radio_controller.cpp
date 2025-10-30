#include "radio_control/radio_controller.hpp"
#include "radio_control/radio_configuration.hpp"

namespace cotsbotics::radio_control
{
    using RC_Channels = RadioConfiguration::Channels;
    using OutputRanges = RadioConfiguration::OutputRanges;

    RadioController::RadioController(cotsbotics::SbusReceiver &sbus_receiver,
                                     MowerControlOutputManager &control_output_manager,
                                     MowerManualControlInputManager &manual_control_input_manager)
        : _sbus_receiver(sbus_receiver),
          _control_output_manager(control_output_manager),
          _manual_control_input_manager(manual_control_input_manager)
    {
    }

    void RadioController::tick()
    {
        // Read SBUS data and update control outputs accordingly
        if (!_sbus_receiver.no_data())
        {
            uint16_t left_throttle = getChannelValueRaw(RC_Channels::LEFT_STICK_Y);
            uint16_t right_throttle = getChannelValueRaw(RC_Channels::RIGHT_STICK_Y);
            auto &state = _control_output_manager.getState();
            state.left_motor.throttle_position =  _throttle_converter.convert(left_throttle);
            state.right_motor.throttle_position = _throttle_converter.convert(right_throttle);
            state.seat_switch_blade = getChannelSwitchState(RC_Channels::SEAT_SWITCH);
            state.seat_switch_drive = getChannelSwitchState(RC_Channels::SEAT_SWITCH);
            state.blades_enabled = getChannelSwitchState(RC_Channels::BLADE_ENABLE) ;
            state.low_speed_drive = getChannelSwitchState(RC_Channels::LOW_SPEED_DRIVE);
            state.low_speed_cut = getChannelSwitchState(RC_Channels::LOW_SPEED_CUT);
            state.left_motor.zero_switch = getChannelSwitchState(RC_Channels::ZERO_SWITCH);
            state.right_motor.zero_switch = getChannelSwitchState(RC_Channels::ZERO_SWITCH);

        }
    }

    void RadioController::stopAllMotors()
    {
        // Implementation to stop all motors
        auto &state = _control_output_manager.getState();
        state.left_motor.throttle_position = OutputRanges::NEUTRAL_OUTPUT;
        state.right_motor.throttle_position = OutputRanges::NEUTRAL_OUTPUT;
        state.seat_switch_blade = MowerSwitch::OPEN;
        state.seat_switch_drive = MowerSwitch::OPEN;    
        state.blades_enabled = MowerSwitch::OPEN;
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
    MowerSwitch RadioController::getChannelSwitchState(int8_t channel)
    {
        return getChannelValueBool(channel) ? MowerSwitch::CLOSED : MowerSwitch::OPEN;
    }

}; // namespace cotsbotics::mower_controller