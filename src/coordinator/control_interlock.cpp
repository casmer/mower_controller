// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>


#include "coordinator/control_interlock.hpp"
#if defined(ARDUINO)
#define DEBUG_MSG(msg)
#else

#include <iostream>
#define DEBUG_MSG(msg) std::cout << msg << std::endl;
#endif
namespace cotsbotics
{
    namespace coordinator
    {

        ControlInterlock::ControlInterlock(
            IDigitalInputPort& interlock_a,
            IDigitalInputPort& interlock_b,
            IDigitalOutputPort& interlock_a2,
            IDigitalOutputPort& interlock_b2) : 
                                                           _interlock_a(interlock_a),
                                                           _interlock_b(interlock_b),
                                                           _interlock_out_a(interlock_a2),
                                                           _interlock_out_b(interlock_b2) {
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
            
            determineControlInterlock();
        };

        

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
                bool interlock_engaged = ((_interlock_a_buffer[_interlock_buffer_index] == _interlock_a_output) &&
                                        (_interlock_b_buffer[_interlock_buffer_index] != _interlock_a_output));
                                        DEBUG_MSG("Interlock Engaged: " << interlock_engaged);
                if (interlock_engaged)
                {
                    // if it does, verify buffer history is correct.
                    bool fault_found = false;
                    for (int i = 0; i < kInterlockBufferSize; i++)
                    {
                        int next_i = (i + 1) % kInterlockBufferSize;
                         DEBUG_MSG("Interlock Fault detected at buffer index: " << i
                            << "\n _interlock_a_buffer[i]: " << _interlock_a_buffer[i]
                            << "\n _interlock_b_buffer[i]: " << _interlock_b_buffer[i] 
                            << "\n next_i: " << next_i
                            << "\n _interlock_a_buffer[next_i]: " << _interlock_a_buffer[next_i]
                            << "\n _interlock_b_buffer[next_i]: " << _interlock_b_buffer[next_i]);
                        DEBUG_MSG("_interlock_a_buffer: [" << _interlock_a_buffer[0] << ", " << _interlock_a_buffer[1] << ", " << _interlock_a_buffer[2] << ", " << _interlock_a_buffer[3] << ", " << _interlock_a_buffer[4] << ", "
                                    << _interlock_a_buffer[5] << ", " << _interlock_a_buffer[6] << ", " << _interlock_a_buffer[7] << ", " << _interlock_a_buffer[8] << ", " << _interlock_a_buffer[9] << "]");
                        DEBUG_MSG("_interlock_b_buffer: [" << _interlock_b_buffer[0] << ", " << _interlock_b_buffer[1] << ", " << _interlock_b_buffer[2] << ", " << _interlock_b_buffer[3] << ", " << _interlock_b_buffer[4] << ", "
                                    << _interlock_b_buffer[5] << ", " << _interlock_b_buffer[6] << ", " << _interlock_b_buffer[7] << ", " << _interlock_b_buffer[8] << ", " << _interlock_b_buffer[9] << "]");

                        if ((_interlock_a_buffer[i] == _interlock_b_buffer[i]) ||
                            (_interlock_a_buffer[i] == _interlock_a_buffer[next_i])||
                            (_interlock_b_buffer[i] == _interlock_b_buffer[next_i]))
                        {
                           
                            // fault condition
                            fault_found = true;
                            break;
                        }
                    }

                    if (fault_found)
                    {
                        _interlock_state = InterlockState::Fault;
                    } 
                    else
                    {
                        _interlock_state = InterlockState::Engaged;
                    }
                }
                else
                {

                     // if it does, verify buffer history is correct.
                    bool fault_found = false;
                    for (int i = 0; i < kInterlockBufferSize; i++)
                    {
                        int next_i = (i + 1) % kInterlockBufferSize;
                        DEBUG_MSG("Interlock Fault detected at buffer index: " << i
                            << "\n _interlock_a_buffer[i]: " << _interlock_a_buffer[i]
                            << "\n _interlock_b_buffer[i]: " << _interlock_b_buffer[i] 
                            << "\n next_i: " << next_i
                            << "\n _interlock_a_buffer[next_i]: " << _interlock_a_buffer[next_i]
                            << "\n _interlock_b_buffer[next_i]: " << _interlock_b_buffer[next_i]);
                        DEBUG_MSG("_interlock_a_buffer: [" << _interlock_a_buffer[0] << ", " << _interlock_a_buffer[1] << ", " << _interlock_a_buffer[2] << ", " << _interlock_a_buffer[3] << ", " << _interlock_a_buffer[4] << ", "
                                    << _interlock_a_buffer[5] << ", " << _interlock_a_buffer[6] << ", " << _interlock_a_buffer[7] << ", " << _interlock_a_buffer[8] << ", " << _interlock_a_buffer[9] << "]");
                        DEBUG_MSG("_interlock_b_buffer: [" << _interlock_b_buffer[0] << ", " << _interlock_b_buffer[1] << ", " << _interlock_b_buffer[2] << ", " << _interlock_b_buffer[3] << ", " << _interlock_b_buffer[4] << ", "
                                    << _interlock_b_buffer[5] << ", " << _interlock_b_buffer[6] << ", " << _interlock_b_buffer[7] << ", " << _interlock_b_buffer[8] << ", " << _interlock_b_buffer[9] << "]");

                        if ((_interlock_a_buffer[i] == _interlock_b_buffer[i]) &&
                            (_interlock_a_buffer[i] == false))
                        {
                            // fault condition
                            fault_found = true;
                            break;
                        }
                    }

                    if (fault_found)
                    {
                        _interlock_state = InterlockState::Fault;
                    } 
                    else
                    {
                        DEBUG_MSG("Interlock Disengaged");
                        _interlock_state = InterlockState::Disengaged;
                    }

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