// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "Arduino.h"
#include "coordinator/control_interlock.hpp"
#if defined(ARDUINO)
#define DEBUG_MSG(msg)
#define DEBUG_MSG_A(msg) Serial.println(msg);
#else

#include <iostream>
#define DEBUG_MSG(msg) std::cout << msg << std::endl;
#define DEBUG_MSG_A(msg) std::cout << msg << std::endl;
#endif
namespace cotsbotics
{
    namespace coordinator
    {

        ControlInterlock::ControlInterlock(
            IDigitalInputPort& interlock_a,
            IDigitalInputPort& interlock_b,
            IDigitalOutputPort& interlock_a2,
            IDigitalOutputPort& interlock_b2,
            int tick_delay_ticks) : 
                                                           _interlock_a(interlock_a),
                                                           _interlock_b(interlock_b),
                                                           _interlock_out_a(interlock_a2),
                                                           _interlock_out_b(interlock_b2),
                                                           _tick_delay_ticks(tick_delay_ticks) {
                                                           };

        void ControlInterlock::setup()
        {
            _interlock_a.setup();
            _interlock_b.setup();
            _interlock_out_a.setup();
            _interlock_out_b.setup();
            // Initialize output signals
            _interlock_a_output = false;
            _interlock_out_a.write(_interlock_a_output);
            _interlock_out_b.write(!_interlock_a_output);
        };

        void ControlInterlock::tick()
        {

            _interlock_a.tick();
            _interlock_b.tick();
            _tick_count++;
            if (_tick_count >= _tick_delay_ticks) 
            {
                _tick_count = 0;
                determineControlInterlock();
            }
        };

        #define btos(b) ((b) ? "1" : "0")
        void ControlInterlock::printBuffers()
        {
            Serial.print("Interlock A: ");
            Serial.print(btos(_interlock_a_output));
            Serial.print(" Buffer: ");
            for (int i = 0; i < kInterlockBufferSize; i++)
            {
                Serial.print(btos(_interlock_a_buffer[i]));
                Serial.print(",");
            }
            Serial.println();
            Serial.print("Interlock B: ");
            Serial.print(btos(!_interlock_a_output));
            Serial.print(" Buffer: ");
            for (int i = 0; i < kInterlockBufferSize; i++)
            {
                Serial.print(btos(_interlock_b_buffer[i]));
                Serial.print(",");
            }
            Serial.println();
        }
        void ControlInterlock::determineControlInterlock()
        {
            // Logic to determine control mode based on interlock states
            _interlock_a_buffer[_interlock_buffer_index] = _interlock_a.read();
            _interlock_b_buffer[_interlock_buffer_index] = _interlock_b.read();
            DEBUG_MSG("Interlock Index: " << _interlock_buffer_index 
                      << " A: " << _interlock_a_buffer[_interlock_buffer_index]
                      << " B: " << _interlock_b_buffer[_interlock_buffer_index]
                      << " Expected A: " << _interlock_a_output
                      << " Expected B: " << !_interlock_a_output);
            if (_interlock_buffer_index == kInterlockBufferSize - 1)
            {
                DEBUG_MSG("Interlock signal buffer is now ready.");
                _interlock_signal_ready = true;
            };

            if (!_interlock_signal_ready)
            {
                DEBUG_MSG("Interlock signal buffer not ready yet.");
                // Not enough data yet
                _interlock_state = InterlockState::Unknown;
            }
            else
            {
                // does the current interlock input match the expected values set last tick.


                int interlock_a_sum = 0;
                int interlock_b_sum = 0;
                bool interlock_a_alternating = true;
                bool interlock_b_alternating = true;
                for (int i = 0; i < kInterlockBufferSize; i++)
                {
                    int next_i = (i + 1) % kInterlockBufferSize;
                    
                    DEBUG_MSG("_interlock_a_buffer: [" << _interlock_a_buffer[0] << ", " << _interlock_a_buffer[1] << ", " << _interlock_a_buffer[2] << ", " << _interlock_a_buffer[3] << ", " << _interlock_a_buffer[4] << ", "
                                << _interlock_a_buffer[5] << ", " << _interlock_a_buffer[6] << ", " << _interlock_a_buffer[7] << ", " << _interlock_a_buffer[8] << ", " << _interlock_a_buffer[9] << "]");
                    DEBUG_MSG("_interlock_b_buffer: [" << _interlock_b_buffer[0] << ", " << _interlock_b_buffer[1] << ", " << _interlock_b_buffer[2] << ", " << _interlock_b_buffer[3] << ", " << _interlock_b_buffer[4] << ", "
                                << _interlock_b_buffer[5] << ", " << _interlock_b_buffer[6] << ", " << _interlock_b_buffer[7] << ", " << _interlock_b_buffer[8] << ", " << _interlock_b_buffer[9] << "]");

                    interlock_a_sum += _interlock_a_buffer[i] ? 1 : 0;
                    interlock_b_sum += _interlock_b_buffer[i] ? 1 : 0;
                    if (_interlock_a_buffer[i] == _interlock_a_buffer[next_i])
                    {
                        interlock_a_alternating = false;
                    }
                    if (_interlock_b_buffer[i] == _interlock_b_buffer[next_i])
                    {
                        interlock_b_alternating = false;
                    }

                }
                if (interlock_a_alternating && (!interlock_b_alternating))
                {
                    if (_interlock_a_buffer[_interlock_buffer_index] != _interlock_a_output)
                    {
                        DEBUG_MSG("Interlock A Alternating but last value does not match expected output. Fault.");
                        _interlock_state = InterlockState::Fault;
                    }
                    else if (interlock_b_sum == kInterlockBufferSize)
                    {
                        _interlock_state = InterlockState::Disengaged;
                        DEBUG_MSG("Interlock A Alternating, Interlock B High: Disengaged");
                    }
                    else 
                    {
                        DEBUG_MSG("Interlock A Alternating, Interlock B Not High: Fault");
                        _interlock_state = InterlockState::Fault;
                    }
                }
                else if ((!interlock_a_alternating) && interlock_b_alternating)
                {
                    if (_interlock_b_buffer[_interlock_buffer_index] == _interlock_a_output)
                    {
                        DEBUG_MSG("Interlock B Alternating but last value does not match expected output. Fault.");
                        _interlock_state = InterlockState::Fault;
                    }
                    else if (interlock_a_sum == kInterlockBufferSize)
                    {
                        _interlock_state = InterlockState::Engaged;
                        DEBUG_MSG("Interlock B Alternating, Interlock A High: Engaged");
                    }
                    else 
                    {
                        DEBUG_MSG("Interlock B Alternating, Interlock A Not High: Fault");
                        _interlock_state = InterlockState::Fault;
                    }
                }
                else
                {
                    DEBUG_MSG("Interlock Signal Fault, neither A nor B are alternating.");
                    _interlock_state = InterlockState::Fault;
                }
            }
             // Toggle the output signals to indicate interlock status
            _interlock_buffer_index = (_interlock_buffer_index + 1) % kInterlockBufferSize;
            _interlock_a_output = !_interlock_a_output;
            _interlock_out_a.write(_interlock_a_output);
            _interlock_out_b.write(!_interlock_a_output);
        
        };

        InterlockState ControlInterlock::interlockState() const
        {
            return _interlock_state;
        };

    } // namespace coordinator
} // namespace cotsbotics   