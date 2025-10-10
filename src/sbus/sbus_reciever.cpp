// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>

#include "sbus/sbus_receiver.hpp"

namespace cotsbotics
{

void SbusReceiver::Begin()
        {
            /* Begin the SBUS communication */
            sbus_rx.Begin();
        }

        void SbusReceiver::tick()
        {
            if (sbus_rx.Read()) {
                /* Grab the received data */
                bfs::SbusData new_data;
                new_data = sbus_rx.data();
                bool data_changed = false;
                if (new_data.lost_frame != data.lost_frame) data_changed = true;
                data.lost_frame = new_data.lost_frame;
                if (new_data.failsafe != data.failsafe) data_changed = true;
                data.failsafe = new_data.failsafe;
                for (int8_t i = 0; i < data.NUM_CH; i++) {
                    
                    if (max(new_data.ch[i], data.ch[i]) - min(new_data.ch[i], data.ch[i]) > 2)
                    {
                        data_changed = true;
                        data.ch[i] = new_data.ch[i];
                    }
                }
                

                if (data_changed)
                {
                    /* Display the received data */
                    Serial.print("sbsus:");
                    for (int8_t i = 0; i < data.NUM_CH; i++) {
                        Serial.print(data.ch[i]);
                        Serial.print("\t");
                    }
                    /* Display lost frames and failsafe data */                   
                    Serial.print(data.lost_frame?"LF":"--");
                    Serial.print("\t");
                    Serial.print(data.failsafe?"FS":"--");
                    Serial.println(":sb");
                    Serial.flush();
                }
                m_no_data_received=false;
            }
            else
            {
                m_no_data_received=true;
            }
        };

}; // cotsbotics
