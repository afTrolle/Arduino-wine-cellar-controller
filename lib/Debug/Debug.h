//only include once
#ifndef DEBUG_H
#define DEBUG_H


//DEBUG IF

//#define DEBUG
//#define DEBUG_WEATHER

#ifdef DEBUG
#include <SPI.h>
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTCT(x) Serial.println(F(x))
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINTCT(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif


#endif