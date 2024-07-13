#include "main.h"

void initServer();
void loopServer();

void init_weather_sensor();
void loop_weather_sensor();

unsigned long weather_read_millis = 0;
float weather_relative_humidity = -1; 
float weather_temperature = -1;

DHT_Unified dht(DHTPIN, DHTTYPE);

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
  init_weather_sensor();
  initServer();
}

void loop()
{
  //updates trip Sensor
  loop_weather_sensor();
  loopServer();
  door_relay_tick();
}

// WebServer

unsigned long static dhcpUpdateMillis;
EthernetServer server(80);

void initServer()
{
  DEBUG_PRINTCT("starting server");
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

    if (strcmp_P(request->path, weather_str) == 0)
    {
        print_http_repsonse(client, request, (char *)http_response_ok);
        print_header(client, F("Content-Type: application/json\r\n"));
        print_header(client, F("Transfer-Encoding: identity\r\n"));
        print_header(client, F("Access-Control-Allow-Origin: *\r\n"));
        print_header(client, F("Access-Control-Allow-Methods: * \r\n"));
        print_end_of_header(client);

        client->print(F("{\n\"temperature\":"));
        client->print(weather_temperature);
        client->print(F("\n,\"humidity\": "));
        client->print(weather_relative_humidity);
        client->print(F("\n}"));
    }
      else
    {
      print_http_repsonse(client, request, (char *)http_response_not_found);
      print_end_of_header(client);
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
      print_header(client, F("Access-Control-Allow-Methods: * \r\n"));
      print_end_of_header(client);

      bool state = relay_action(true);
      if (state)
      {
        print_header(client, F("{\"status\":200, \"message\":\"ok\"}")); // ok
      }
      else
      {
        print_header(client, F("{\"status\":300, \"message\":\"door already in use\"}")); // seesion already ongoing
      }
    }
    else if (strcmp_P(request->path, close_relaystr) == 0)
    {
      print_http_repsonse(client, request, (char *)http_response_ok);
      print_header(client, F("Access-Control-Allow-Origin: * \r\n"));
      print_header(client, F("Access-Control-Allow-Methods: * \r\n"));
      print_end_of_header(client);

      bool state = relay_action(false);
      if (state)
      {
        print_header(client, F("{\"status\":200, \"message\":\"ok\"}")); // ok
      }
      else
      {
        print_header(client, F("{\"status\":300, \"message\":\"door already in use\"}")); //session already ongoing
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
      // if error print error
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


void init_weather_sensor() {
  // Initialize device.
  dht.begin();
}

void loop_weather_sensor() {
  unsigned long now = millis();
  if (weather_read_millis + 10000  < now) {
    weather_read_millis = now;
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      weather_temperature = event.temperature;
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
      weather_relative_humidity = event.relative_humidity;
    }
  }
}
