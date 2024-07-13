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
#include "Debug.h"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTTYPE DHT11     
#define DHTPIN 5

#define DHCP_UPDATE_INTERVAL 10000
static unsigned char MAC_ADDRESS[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

//rest api
const static char open_relaystr[] PROGMEM = "/open";
const static char close_relaystr[] PROGMEM = "/close";
const static char weather_str[] PROGMEM = "/weather";

#endif