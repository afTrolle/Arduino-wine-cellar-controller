#include "main.h"

void initServer();
void loopServer();

void door_relay_tick(bool is_door_triggered);
bool relay_action(bool too_open, bool force);

#ifdef DEBUG
const static int SerialFrequency = 9600;
void initSerial()
{
  Serial.begin(SerialFrequency);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}
#endif

void setup()
{

#ifdef DEBUG
  initSerial();
#endif
  init_relay();
  init_clamp();
  initServer();
  init_weather();
  initSD();
}

void loop()
{

  //updates trip Sensor

  door_relay_tick(read_sensor());
  loopServer();

  door_relay_tick(read_sensor());

  //updates trip Sensor
  weather_tick();
}

// WebServer

unsigned long static dhcpUpdateMillis;
EthernetServer server(80);

void initServer()
{
  unsigned char state = Ethernet.begin(MAC_ADDRESS);

  // if DHCP failed
  if (state == 0)
  {
    DEBUG_PRINTCT("Failed to configure Ethernet using DHCP");
  }
  else
  {
    DEBUG_PRINTLN(Ethernet.localIP());
    dhcpUpdateMillis = millis();
    server.begin();
  }
}

void updateDHCP()
{
  unsigned long currentMillis = millis();
  if (currentMillis - dhcpUpdateMillis > DHCP_UPDATE_INTERVAL)
  {
    Ethernet.maintain();
    dhcpUpdateMillis = millis();
  }
}

void handleRequest(struct httpRequest *request, EthernetClient *client)
{
  if (request->method == http_method::GET)
  {
    DEBUG_PRINT(F("GET called: "));
    DEBUG_PRINTLN(request->path);

    if (strncmp_P(request->path, weatherPath, strlen_P(weatherPath)) == 0)
    {
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Content-Type: application/json\r\n"));
      print_header(client, F("Transfer-Encoding: identity\r\n"));
      print_header(client, F("Access-Control-Allow-Origin: *\r\n"));
      print_end_of_header(client);
      client->print(F("{\n\"temperature\":"));
      client->print(current_temperature);
      client->print(F("\n,\"humidity\": "));
      client->print(current_humdidity);
      client->print(F("\n}"));
    }
    else if (strncmp_P(request->path, door_sensor, strlen_P(door_sensor)) == 0)
    {
      //Get door sensor data
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Content-Type: application/json\r\n"));
      print_header(client, F("Transfer-Encoding: identity\r\n"));
      print_header(client, F("Access-Control-Allow-Origin: *\r\n"));
      print_end_of_header(client);
      client->print(F("["));

      int *clamp_sensor_data = get_clamp_readings();
      for (int i = 0; i < (SAMPLE_SIZE - 2); i++)
      {
        client->print(clamp_sensor_data[i], 10);
        client->print(F(","));
      }
      client->print(clamp_sensor_data[SAMPLE_SIZE - 1]);

      client->print(F("]"));
    }
    else if (strcmp_P(request->path, indexPath) == 0)
    {
      //SEND INDEX.html
      printFile(F("/index.htm"), client, request);
    }
    else if (strchr(request->path, '.') == NULL)
    {
      int newbuff = strlen(request->path);
      char charbuff[newbuff + LENGTH(addition)];
      strcpy(charbuff, request->path);
      strcat_P(charbuff, addition);
      printFile(charbuff, client, request);
    }
    else
    {
      printFile(request->path, client, request);
    }
  }
  else if (request->method == http_method::POST)
  {
    DEBUG_PRINT(F("POST called: "));
    DEBUG_PRINTLN(request->path);
    if (strcmp_P(request->path, open_relaystr) == 0)
    {
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Access-Control-Allow-Origin: * \r\n"));
      print_end_of_header(client);

      if (is_triggered == false)
      {
        const boolean success = relay_action(true, false);
        if (success)
        {
          //ok
          print_header(client, F("{\"status\":200, \"message\":\"ok\"}"));
        }
        else
        {
          print_header(client, F("{\"status\":300, \"message\":\"door already in use\"}"));
          //seesion already ongoing
        }
      }
      else
      {
        print_header(client, F("{\"status\":400, \"message\":\"Door saftey tripped\"}"));
        //triped emergerncy
      }
    }
    else if (strcmp_P(request->path, close_relaystr) == 0)
    {
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Access-Control-Allow-Origin: * \r\n"));
      print_end_of_header(client);

      if (is_triggered == false)
      {
        boolean state = relay_action(false, false);
        if (state)
        {
          print_header(client, F("{\"status\":200, \"message\":\"ok\"}")); //ok
        }
        else
        {
          print_header(client, F("{\"status\":300, \"message\":\"door already in use\"}")); //seesion already ongoing
        }
      }
      else
      {
        print_header(client, F("{\"status\":400, \"message\":\"Door saftey tripped\"}"));
        //triped emergerncy
      }
    }
    else if (strcmp_P(request->path, force_open_relaystr) == 0)
    {
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Access-Control-Allow-Origin: * \r\n"));
      print_end_of_header(client);

      bool state = relay_action(true, true);
      if (state)
      {
        print_header(client, F("{\"status\":200, \"message\":\"ok\"}")); //ok
      }
      else
      {
        print_header(client, F("{\"status\":300, \"message\":\"door already in use\"}")); //seesion already ongoing
      }
    }
    else if (strcmp_P(request->path, force_close_relaystr) == 0)
    {
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Access-Control-Allow-Origin: * \r\n"));
      print_end_of_header(client);

      bool state = relay_action(false, true);
      if (state)
      {
        print_header(client, F("{\"status\":200, \"message\":\"ok\"}")); //ok
      }
      else
      {
        print_header(client, F("{\"status\":300, \"message\":\"door already in use\"}")); //seesion already ongoing
      }
    }
    else
    {
      print_http_repsonse(client, request, (char *)http_response_not_found);
      print_end_of_header(client);
    }
  }
}

void loopServer()
{
  EthernetClient client = server.available();

  if (client)
  {
    //might be a bit too aggresive but don't want web server to block sensor!
    client.setTimeout(500);

    struct httpRequest *request = parse_http_request(&client);
    DEBUG_PRINTLN(request->path);
    if (request->http_error_progmem_pointer != NULL)
    {
      //if error print error
      DEBUG_PRINT("http_error_progmem_pointer is not  NULL");
      print_http_repsonse(&client, request, request->http_error_progmem_pointer);
      print_end_of_header(&client);
    }
    else
    {
      DEBUG_PRINT("http_error_progmem_pointer is NULL");
      handleRequest(request, &client);
    }

    delay(2);
    client.flush();
    client.stop();
  }
}

//milis that force open/close is opened
#define action_recvied_elpased 600
bool is_forcing = false;
unsigned long relay_action_recived_millis = 0;

bool relay_action(bool too_open, bool force)
{
  if (is_triggered && !force)
  {
    return false;
  }

  //if already running
  if (is_power_pin_high)
  {
    //same args called as running then update millis
    if (is_forcing == force && is_open_or_close_pin_open == too_open)
    {
      relay_action_recived_millis = millis();
      return true;
    }
  }
  else
  {
    toggle_on_relay(too_open);
    is_forcing = force;
    relay_action_recived_millis = millis();
    return true;
  }
  return false;
}

//called as often as possible
//desides when it is time to stop
void door_relay_tick(bool is_door_triggered)
{
  //Is relay on
  if (is_power_pin_high)
  {
    //Is forcing opening
    if (is_forcing)
    {
      //force recived millies need to be updated
      if ((millis() - relay_action_recived_millis) > action_recvied_elpased)
      {
        toggle_off_relay();
      }
    }
    else
    {
      //not forcing
      if (is_door_triggered)
      {
        //triggered saftey
        toggle_off_relay();
      }
      else if ((millis() - relay_action_recived_millis) > action_recvied_elpased)
      {
        //no saftey triggered but stop hasn't recvied too keep going
        toggle_off_relay();
      }
    }
  }
}