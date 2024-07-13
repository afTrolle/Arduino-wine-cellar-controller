//only include once
#ifndef _RELAY_CONTROLLER_H
#define _RELAY_CONTROLLER_H

#include <Arduino.h>

//power to relay for opening and closing relay
#define POWER_PIN 7
//define if power is going to open or close command
#define OPEN_CLOSE_PIN 9

void init_relay();
void door_relay_tick();
bool relay_action(bool too_open);

#endif