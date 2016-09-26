#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>

class DHTManager {
private:
  float data[3];
  DHT *dht;

  float _lastTemperature;
  float _lastHumidity;
  float _lastHeatIndex;

public:
  DHTManager(int dhtPin, int dhtType);
  ~DHTManager();
  float *getData();
};

#endif
