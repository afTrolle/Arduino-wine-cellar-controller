
#include "SD_Controller.h"


void initSD()
{
  DEBUG_PRINTCT("initinalizing SD");
  //init sd card, false if fails to open sd card
  if (!SD.begin(4))
  {
    DEBUG_PRINTCT("sd failed to open");
  }
}

void printFile(const String dir, EthernetClient *client, struct httpRequest *request)
{

  if (SD.exists(dir))
  {
    DEBUG_PRINTLN("opening file");
    File file = SD.open(dir);
    if (file)
    {

      if (!file.isDirectory())
      {
        DEBUG_PRINTLN("opened file");
        print_http_repsonse(client, request, (char *)http_response_ok);
        print_end_of_header(client);
        int bytesToRead = file.available();
        const int buffer_size = 100;
        char buffer[buffer_size];
        while (bytesToRead)
        {
          if (buffer_size > bytesToRead)
          {
            file.readBytes(buffer, bytesToRead);
            client->write(buffer, bytesToRead);
          }
          else
          {
            file.readBytes(buffer, buffer_size);
            client->write(buffer, buffer_size);
          }
          bytesToRead = file.available();
        }
      }
      else
      {
        //incorrect file type
        print_http_repsonse(client, request, (char *)http_response_not_found);
        print_end_of_header(client);
      }
      file.close();
    }
  }
  else
  {
    //failed to open file
    print_http_repsonse(client, request, (char *)http_response_not_found);
    print_end_of_header(client);
  }
}