#ifndef DHT_MANAGER_H
#define DHT_MANAGER_H

#include <DHT.h>

class DHTManager {
private:
  DHT _dht;
public:
  struct DHTData {
    float temperature;
    float humidity;
    float heatIndex;
  };
  DHTManager(int pin, int type);
  bool getData(DHTData &data, bool isExternalTemperature = false, float externalTemperature = 1.0);
};

#endif
