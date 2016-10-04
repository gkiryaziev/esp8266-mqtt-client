
#ifndef HTTP_MANAGER_H
#define HTTP_MANAGER_H

#include <ESP8266HTTPClient.h>

class HTTPManager {
public:
  int GET(String url);
  int POST(String url, String payload);
};

#endif
