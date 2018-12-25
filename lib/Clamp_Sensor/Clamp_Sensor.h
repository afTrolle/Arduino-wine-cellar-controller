//only include once
#ifndef _CLAMP_SENSOR_H
#define _CLAMP_SENSOR_H

#include <Arduino.h>

#define NOICE_TOLLERNACE 80
#define SAMPLE_SIZE 10
#define SENSOR_PIN PIN_A0

//read this var to see if triggered!
//Observe Never modify outside of clampLib.cpp!!!
extern bool is_triggered;

//init clamp checking
void init_clamp();
//check if triggered and stores is in sample list
bool read_sensor();
// get triggger buffer! Not in order!
int *get_clamp_readings();

#endif