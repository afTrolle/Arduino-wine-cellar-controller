#include "webServer.h"
#include <Arduino.h>


#define LENGTH(x) (sizeof(x)/sizeof(*(x)))

//#define DEBUG

#ifdef DEBUG
#include <SPI.h>
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTCT(x)  Serial.println(F(x))
#define DEBUG_PRINTLN(x)  Serial.println(x)
#else
#define DEBUG_PRINTCT(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif


const static char  http_get[] PROGMEM  = "GET";
const static char http_post[] PROGMEM = "POST";

const static char http_one_one_ver[] PROGMEM = "HTTP/1.1";
const static char http_one_zero[] PROGMEM = "HTTP/1.0";


static char httpReqBuffer[HTTP_BUFFER_SIZE];

size_t readLine(EthernetClient * client);
void findMethod(char * pMethod , struct httpRequest * request);
void findVersion(char * pVersion , struct httpRequest * request);
void readRequestLine(EthernetClient * client,	struct httpRequest * request);

/** 		Public functions       **/


struct  httpRequest req;

void clearRequest(){
	req.method = http_method::UNKNOWN;
	req.version = http_version::UN_KNOWN;
	req.http_error_progmem_pointer = NULL;
	req.path[0] = '\0';
	req.query[0] =  '\0';
	req.path[HTTP_PATH_SIZE-1] = '\0';
	req.query[HTTP_QUERY_SIZE-1] =  '\0';
}

struct httpRequest * parse_http_request(EthernetClient * client){
	clearRequest();
	readRequestLine(client,	&req);
	DEBUG_PRINTLN(httpReqBuffer);
	return &req;
}


void print_http_repsonse(EthernetClient * client, struct httpRequest * request, char * http_response_header){
	char strBuffer[50];
	strcpy_P(strBuffer, http_response_header);
	client->print(F("HTTP/1.1 "));
	client->print(strBuffer);
	client->print(F("\r\n"));
}

void print_header(EthernetClient * client, const __FlashStringHelper* headerStr){
	client->print(headerStr);
}

void print_end_of_header(EthernetClient * client){
	client->print("\r\n");
}

/****   private functions            ******/

static const char  pSpDelimiters[] PROGMEM = " \r\n";
static const char queryDelimiter[] PROGMEM = "?";

void readRequestLine(EthernetClient * client,	struct httpRequest * request )
{
	size_t numBytesRead = readLine(client);

	DEBUG_PRINT("print read buffer:");
	DEBUG_PRINTLN(httpReqBuffer);


	//if error reading first line
	if (numBytesRead == 0){
		//error reading first line
		request->http_error_progmem_pointer = (char *) http_response_bad_Request;
		return;
	}

	//Request-Line = Method SP Request-URI SP HTTP-Version CRLF
	// Split it into the 3 tokens

	char * pMethod  = strtok_P(httpReqBuffer, pSpDelimiters);
	char * pUri     = strtok_P(NULL, pSpDelimiters);
	char * pVersion = strtok_P(NULL, pSpDelimiters);

	if (pMethod == NULL || pUri == NULL || pVersion == NULL){
		request->http_error_progmem_pointer =	(char *) http_response_bad_Request;
		return;
	}

	// URI may optionally comprise the URI of a queryable object a '?' and a query
	// see http://www.ietf.org/rfc/rfc1630.txt
	strtok_P(pUri, queryDelimiter);
	char * pQuery   = strtok_P(NULL, queryDelimiter);
	if (pQuery != NULL)
	{
		strncpy(request->query, pQuery, HTTP_QUERY_SIZE-1);
	}

	findVersion(pVersion ,request);
	if (request->http_error_progmem_pointer != NULL){
		return;
	}

	findMethod(pMethod,request);
	if (request->http_error_progmem_pointer != NULL){
		return;
	}

	strncpy(request->path, pUri, HTTP_PATH_SIZE-1);
}


size_t readLine(EthernetClient * client) {
	int bytesAvailable = 	client->available();
	if (bytesAvailable) {
		int bytesLeft = HTTP_BUFFER_SIZE;
		unsigned int bytesToRead;
		if (bytesAvailable < bytesLeft){
			bytesToRead = bytesAvailable;
		} else {
			bytesToRead = bytesLeft;
		}
		
		size_t numBytesRead =	client->readBytesUntil('\n', httpReqBuffer, bytesToRead);
		if (bytesToRead == numBytesRead || numBytesRead == 0){
			//failed to find new line
			DEBUG_PRINT("failed To read");
			return 0;
		}
		httpReqBuffer[numBytesRead-1] = '\0'; //set end of string //overrite \n
		return numBytesRead;
	}
	// failed to find new line
	return 0;
}

void findMethod(char * pMethod , struct httpRequest * request){
	if (strncmp_P(pMethod, http_get,LENGTH(http_get)) == 0){
		request->method = http_method::GET;
	} else if (strncmp_P(pMethod, http_post,LENGTH(http_post)) == 0){
		request->method = http_method::POST;
	} else {
		DEBUG_PRINT("Error didn't find compatiable method");
		request->method = http_method::UNKNOWN;
		request->http_error_progmem_pointer =  (char *)http_response_not_implemented;
	}
}


void findVersion(char * pVersion , struct httpRequest * request){
	if (strncmp_P(pVersion, http_one_one_ver,LENGTH(http_one_one_ver)) == 0){
		request->version = http_version::ONE_ONE;
	} else if (strncmp_P(pVersion, http_one_zero,LENGTH(http_one_zero)) == 0){
		request->version = http_version::ONE_ZERO;
		request->http_error_progmem_pointer =  (char *)http_response_not_supported;
	} else {
		request->http_error_progmem_pointer =  (char *)http_response_not_supported;
	}
}
