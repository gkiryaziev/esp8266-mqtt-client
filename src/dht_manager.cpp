#include "dht_manager.h"

DHTManager::DHTManager(int pin, int type) : _dht(pin, type) {
  _lastTemperature = 0;
  _lastHumidity = 0;
  _lastHeatIndex = 0;
  _dht.begin();
}

void DHTManager::getData(DHTData &data, bool isExternalTemperature, float externalTemperature) {

  // get data
  float temperature = _dht.readTemperature(false, false);
  float humidity = _dht.readHumidity(false);

  // check data
  if (isnan(temperature) || isnan(humidity)) {
    data.temperature = _lastTemperature;
    data.humidity = _lastHumidity;
    data.heatIndex = _lastHeatIndex;
    return;
  }

  // compute heat index
  float heatIndex = 0;
  if (isExternalTemperature) {
    heatIndex = _dht.computeHeatIndex(externalTemperature, humidity, false);
  } else {
    heatIndex = _dht.computeHeatIndex(temperature, humidity, false);
  }

  // save last data
  data.temperature = _lastTemperature = temperature;
  data.humidity = _lastHumidity = humidity;
  data.heatIndex = _lastHeatIndex = heatIndex;
}
