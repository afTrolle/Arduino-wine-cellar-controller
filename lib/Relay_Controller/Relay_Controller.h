//only include once
#ifndef _RELAY_CONTROLLER_H
#define _RELAY_CONTROLLER_H

#include <Arduino.h>

//power to relay for opening and closing relay
#define POWER_PIN 42
//define if power is going to open or close command
#define OPEN_CLOSE_PIN 46

//which state to be in to open or close
#define OPEN_Relay HIGH
#define CLOSE_Relay LOW

//if relay is open
extern bool is_power_pin_high;
//open is same as high
extern bool is_open_or_close_pin_open;
//millis since started to open, could be used for auto open.
extern unsigned long millis_at_relay_activated;

void init_relay();
bool toggle_on_relay(bool too_open);
void toggle_off_relay();

#endif