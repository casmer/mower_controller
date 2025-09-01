#include <Arduino.h>
#include "sbus/sbus_receiver.hpp"
cotsbotics::SbusReceiver sbus_receiver;

void setup() {
  /* Serial to display data */
  Serial.begin(115200);
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_receiver.Begin();
}

void loop () {
  sbus_receiver.tick();

  delay(20);
  if (sbus_receiver.no_data())
  {
    static int print_counter=0;
    print_counter++;
    if (print_counter>=200)
    {
      Serial.write(".");
      print_counter=0;
    }


  }
}