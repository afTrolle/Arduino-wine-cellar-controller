//only include once
#ifndef _WEATHER_CONTROLLER_H
#define _WEATHER_CONTROLLER_H

#include "Debug.h"
#include "SparkFunHTU21D.h"

//how often we should update currentTemp and current_Humdidity
#define WEATHER_UPDATE_INTERVAL 15000

//error message if we failed to read Temperature or Humidity
#define ERROR_I2C_TIMEOUT 998
#define ERROR_BAD_CRC 999

extern float current_humdidity;
extern float current_temperature;

//star serial conncetion to sensor
void init_weather();
//updates weather at interval
void weather_tick();

#endif