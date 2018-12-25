//only include once
#ifndef _web_server_h
#define _web_server_h

#include <avr/pgmspace.h>
#include <Ethernet.h>

#define HTTP_BUFFER_SIZE 150
#define HTTP_PATH_SIZE 50
#define HTTP_QUERY_SIZE 50

enum http_method {GET, HEAD, POST, PUT, DELETE, CONNECT, OPTIONS, TRACE, PATCH, UNKNOWN};
enum http_version {ONE_ONE, ONE_ZERO , UN_KNOWN};

const static char PROGMEM http_response_continute[]  = "100 Continue";
const static char PROGMEM http_response_ok[] = "200 OK";
const static char PROGMEM http_response_accepted[] =  "202 Accepted";
const static char PROGMEM http_response_no_content[] = "204 No Content";
const static char PROGMEM http_response_bad_Request[] = "400 Bad Request";
const static char PROGMEM http_response_not_found[] = "404 Not Found";
const static char PROGMEM http_response_URI_too_large[] = "414 Request-URI Too Large";
const static char PROGMEM http_response_Internal_server_error[] = "500 Internal Server Error" ;
const static char PROGMEM http_response_not_implemented[] = "501 Not Implemented";
const static char PROGMEM http_response_not_supported[] = "505 HTTP Version not supported";


struct httpRequest {
	http_method method;
	http_version version;
	char path[HTTP_PATH_SIZE];
	char query[HTTP_QUERY_SIZE];
	char * http_error_progmem_pointer;
};

struct httpRequest * parse_http_request(EthernetClient * client);
void print_http_repsonse(EthernetClient * client, struct httpRequest * request, char * http_response_header);
void print_header(EthernetClient * client, const __FlashStringHelper* headerStr);
void print_end_of_header(EthernetClient * client);
#endif
