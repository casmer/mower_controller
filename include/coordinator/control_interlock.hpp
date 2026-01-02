// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#pragma once

#include "physical_io/IDigitalInputPort.hpp"
#include "physical_io/IDigitalOutputPort.hpp"
#include "coordinator/enum/interlock_state.hpp"

namespace cotsbotics
{
    namespace coordinator
    {
        class ControlInterlock
        {
        public:
            ControlInterlock(
                IDigitalInputPort &interlock_a,
                IDigitalInputPort &interlock_b,
                IDigitalOutputPort &interlock_a2,
                IDigitalOutputPort &interlock_b2);

            /// @brief Setup the interlock ports
            void setup();

            /// @brief Run periodic tasks for the control interlock
            void tick();

            /// @brief Determine if the remote control interlock is engaged (allowing radio or robotic takeover)
            void determineControlInterlock();
            /// @brief Get the current state of the interlock
            /// @return Interlock State as InterlockState enum
            InterlockState interlockState() const;

            /// @brief Check if the interlock signal buffer is ready
            bool isInterlockSignalReady() const
            {
                return _interlock_signal_ready;
            };
            /// @brief Size of the interlock signal buffer
            constexpr static int kInterlockBufferSize{10};
        private:
            /// @brief Digital input port for interlock A
            IDigitalInputPort &_interlock_a;
            /// @brief Digital input port for interlock B
            IDigitalInputPort &_interlock_b;
            /// @brief Digital output port for interlock A
            IDigitalOutputPort &_interlock_out_a;
            /// @brief Digital output port for interlock B
            IDigitalOutputPort &_interlock_out_b;

            bool _interlock_a_output{false};
            bool _interlock_a_buffer[kInterlockBufferSize] = {0};
            bool _interlock_b_buffer[kInterlockBufferSize] = {0};
            int _interlock_buffer_index {0};
            bool _interlock_signal_ready {false};

            InterlockState _interlock_state{InterlockState::Unknown};

        };
    };
};