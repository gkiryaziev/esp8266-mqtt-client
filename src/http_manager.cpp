
#include "http_manager.h"

int HTTPManager::GET(String url) {
  HTTPClient _http;
  if (!_http.begin(url)) {
    return -1;
  }
  int code = _http.GET();
  _http.end();
  return code;
}

int HTTPManager::POST(String url, String payload) {
  HTTPClient _http;
  if (!_http.begin(url)) {
    return -1;
  }
  _http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int code = _http.POST(payload);
  _http.end();
  return code;
}
