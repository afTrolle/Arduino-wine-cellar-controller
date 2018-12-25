#include "Weather_Controller.h"

HTU21D weatherChip;
unsigned long millisSinceWeatherUpdate = 15000;

float current_humdidity = -1;
float current_temperature = -1;

void init_weather()
{
  weatherChip.begin();
}

void weather_tick()
{
  /* code */
  //Returns 998 if I2C timed out
  //Returns 999 if CRC is wrong

  //  if ((millis() - millisSinceWeatherUpdate) > WEATHER_UPDATE_INTERVAL)
  //  {
  current_humdidity = weatherChip.readHumidity();
  current_temperature = weatherChip.readTemperature();
  millisSinceWeatherUpdate = millis();

#ifdef DEBUG_WEATHER
  DEBUG_PRINT("current weather: Temp ");
  DEBUG_PRINT(current_temperature);
  DEBUG_PRINT(" C, Humidity ");
  DEBUG_PRINTLN(current_humdidity);
#endif
  // }
}
