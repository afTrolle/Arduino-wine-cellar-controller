#include "Relay_Controller.h"

// milis that force open/close is opened
#define action_recvied_elpased 600
unsigned long relay_action_recived_millis = 0;

// if relay is open
bool is_power_pin_high = false;
// open is same as high
bool is_open = false;

void init_relay()
{
  pinMode(POWER_PIN, OUTPUT);        // sets the digital pin as output
  pinMode(OPEN_CLOSE_PIN, OUTPUT);   // sets the digital pin as output
  delay(100); 
  digitalWrite(POWER_PIN, LOW);      // set power pin to low
  digitalWrite(OPEN_CLOSE_PIN, LOW); // can be ignored
}

// called as often as possible
// desides when it is time to stop
void door_relay_tick()
{
  // Is relay on
  if (is_power_pin_high)
  {
    // Force recived millies need to be updated
    if ((millis() - relay_action_recived_millis) > action_recvied_elpased)
    {
      digitalWrite(POWER_PIN, LOW); // this is the important one
      delay(200);                   // should be made async! think It might also be removed, used for let-ing relay turn on
      digitalWrite(OPEN_CLOSE_PIN, LOW);
      is_open = false;
      is_power_pin_high = false;
    }
  }
}

bool relay_action(bool too_open)
{
  // if already running
  if (is_power_pin_high)
  {
    // same args called as running then update millis
    if (is_open == too_open)
    {
      relay_action_recived_millis = millis();
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    is_power_pin_high = true;
    if (too_open)
    {
      is_open = true;
      digitalWrite(OPEN_CLOSE_PIN, HIGH);
      delay(200);                    // should be made async! think It might also be removed, used for let-ing relay turn on
      digitalWrite(POWER_PIN, HIGH); // this is the important one
    }
    else
    {
      is_open = false;
      digitalWrite(OPEN_CLOSE_PIN, LOW);
      delay(200);
      digitalWrite(POWER_PIN, HIGH); // this is the important one
    }

    relay_action_recived_millis = millis();
    return true;
  }
  return false;
}
