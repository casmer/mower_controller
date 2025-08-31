#include <Arduino.h>
#include "sbus.h"

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial2,true);
// bfs::SbusRx sbus_rx(&Serial2,16,17,true);
/* SBUS object, writing SBUS */
bfs::SbusTx sbus_tx(&Serial2,true);
// bfs::SbusTx sbus_tx(&Serial2,16,17,true);
/* SBUS data */
bfs::SbusData data;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_rx.Begin();
  sbus_tx.Begin();
}

void loop () {
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();
    /* Display the received data */
    Serial.println("->");
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
    /* Set the SBUS TX data to the received data */
    sbus_tx.data(data);
    /* Write the data to the servos */
    sbus_tx.Write();
    delay(240);
  }
  else
  {
    static int print_counter=0;
    delay(10);
    print_counter++;
    if (print_counter>=200)
    {
      Serial.write(".");
      print_counter=0;
    }


  }
}