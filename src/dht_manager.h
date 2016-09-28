#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>

class DHTManager {
private:
  DHT _dht;
  float _lastTemperature;
  float _lastHumidity;
  float _lastHeatIndex;
public:
  struct DHTData {
    float temperature;
    float humidity;
    float heatIndex;
  };
  DHTManager(int pin, int type);
  void getData(DHTData &data);
};

#endif
