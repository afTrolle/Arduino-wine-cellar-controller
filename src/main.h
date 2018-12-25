//only include once
#ifndef _MAIN_H
#define _MAIN_H

// definitions

#define LENGTH(x) (sizeof(x) / sizeof(*(x)))

#include <Arduino.h>

#include <Ethernet.h>
#include <avr/pgmspace.h>
#include <Wire.h>

#include "webServer.h"
#include "Relay_Controller.h"
#include "Clamp_Sensor.h"
#include "Weather_Controller.h"
#include "SD_Controller.h"
#include "Debug.h"

#define DHCP_UPDATE_INTERVAL 2000
static unsigned char MAC_ADDRESS[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
//uint8_t *

const static char addition[] PROGMEM = ".htm";

//rest api
const static char weatherPath[] PROGMEM = "/weather";
const static char indexPath[] PROGMEM = "/";

const static char door_sensor[] PROGMEM = "/door";

const static char open_relaystr[] PROGMEM = "/open";
const static char close_relaystr[] PROGMEM = "/close";
const static char force_open_relaystr[] PROGMEM = "/fopen";
const static char force_close_relaystr[] PROGMEM = "/fclose";

#endif