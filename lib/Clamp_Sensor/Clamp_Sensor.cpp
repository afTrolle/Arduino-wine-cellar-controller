#include "Clamp_Sensor.h"

const unsigned long MILLIS_TRIGGER_LASTING = 20000;

//millis clamp was triggered
bool is_triggered = false;

unsigned long millis_at_clamp_trigger = 0;

int clamp_sample_buffer[SAMPLE_SIZE];
int *clamp_sample_pointer = clamp_sample_buffer;

/* Public functions */

//set buffer to zero and set pointer
void init_clamp()
{
  for (size_t i = 0; i < SAMPLE_SIZE; i++)
  {
    clamp_sample_buffer[i] = 0;
  }
  clamp_sample_pointer = clamp_sample_buffer;
}

int *next_position_in_pointer()
{
  if (&(clamp_sample_buffer[SAMPLE_SIZE - 1]) <= clamp_sample_pointer)
  {
    return clamp_sample_buffer;
  }
  else
  {
    return clamp_sample_pointer + 1;
  }
}

bool read_sensor()
{
  int reading = analogRead(SENSOR_PIN);
  clamp_sample_pointer = next_position_in_pointer();
  *clamp_sample_pointer = reading;

  if (reading > NOICE_TOLLERNACE)
  {
    millis_at_clamp_trigger = millis();
    is_triggered = true;
  }
  else
  {
    if ((millis() - millis_at_clamp_trigger) >= MILLIS_TRIGGER_LASTING)
    {
      is_triggered = false;
    }
  }
  return is_triggered;
}

int *get_clamp_readings()
{
  return clamp_sample_buffer;
}

/* Private functions */

//cool way to read sensor
void get_sensor_data_in_order()
{
  int *oldestValue;
  if (&clamp_sample_buffer[SAMPLE_SIZE - 1] == clamp_sample_pointer)
  {
    oldestValue = clamp_sample_buffer;
  }
  else
  {
    oldestValue = clamp_sample_pointer++;
  }

  //print oldest value
  //client->print(*oldestValue, 10);

  //start at next oldest value
  for (int *i = oldestValue++; i <= &clamp_sample_buffer[SAMPLE_SIZE - 1]; i++)
  {
    //print until end of buffer
    //client->print(F(","));
    //client->print(*i, 10);
  }

  //start at start of buffer and go to oldest value
  //is still do a wrap around buffer for newer items
  for (int *i = clamp_sample_buffer; i < oldestValue; i++)
  {
    // client->print(F(","));
    //  client->print(*i, 10);
  }

  //print json end of array
  //client->print(F("]"));
}