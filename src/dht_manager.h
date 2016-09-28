#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>

class DHTManager {
private:
  DHT _dht;
  float _data[3];
  float _lastTemperature;
  float _lastHumidity;
  float _lastHeatIndex;
public:
  DHTManager(int pin, int type);
  float *getData();
};

#endif
