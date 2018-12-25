//only include once
#ifndef SD_CONTROLLER_H
#define SD_CONTROLLER_H

#include "webServer.h"
#include "Debug.h"
#include <SD.h>

void initSD();
void printFile(const String dir, EthernetClient *client, struct httpRequest *request);

#endif