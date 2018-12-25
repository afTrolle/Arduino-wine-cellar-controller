#include "Relay_Controller.h"

//if relay is open
bool is_power_pin_high = false;
//open is same as high
bool is_open_or_close_pin_open = false;
//millis since started to open, could be used for auto open.
unsigned long millis_at_relay_activated = 0;

void init_relay()
{
    pinMode(POWER_PIN, OUTPUT);        // sets the digital pin as output
    pinMode(OPEN_CLOSE_PIN, OUTPUT);   // sets the digital pin as output
    digitalWrite(POWER_PIN, LOW);      //set power pin to low
    digitalWrite(OPEN_CLOSE_PIN, LOW); // can be ignored
}

//call with true to send open realy
//false mean to close
bool toggle_on_relay(bool too_open)
{
    if (is_power_pin_high == false)
    {
        is_power_pin_high = true;
        millis_at_relay_activated = millis();

        if (too_open)
        {
            is_open_or_close_pin_open = true;
            digitalWrite(OPEN_CLOSE_PIN, OPEN_Relay);
            delay(200);                    //should be made async! think It might also be removed, used for let-ing relay turn on
            digitalWrite(POWER_PIN, HIGH); //this is the important one
        }
        else
        {
            digitalWrite(POWER_PIN, HIGH); //this is the important one
        }
        return true;
    }
    return false;
}

void toggle_off_relay()
{
    digitalWrite(POWER_PIN, LOW); //this is the important one
    delay(200);                   //should be made async! think It might also be removed, used for let-ing relay turn on
    digitalWrite(OPEN_CLOSE_PIN, LOW);
    is_open_or_close_pin_open = false;
    is_power_pin_high = false;
}
