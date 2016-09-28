#include "dht_manager.h"

DHTManager::DHTManager(int pin, int type) : _dht(pin, type) {
  _lastTemperature = 0;
  _lastHumidity = 0;
  _lastHeatIndex = 0;
  _dht.begin();
}

float *DHTManager::getData() {
  // get data
  float temperature = _dht.readTemperature(false, false);
  float humidity = _dht.readHumidity(false);

  // check data
  if (isnan(temperature) || isnan(humidity)) {
    _data[0] = _lastTemperature;
    _data[1] = _lastHumidity;
    _data[2] = _lastHeatIndex;
    return _data;
  }

  // compute heat index
  float heatIndex = _dht.computeHeatIndex(temperature, humidity, false); // Celsius

  _data[0] = temperature;
  _data[1] = humidity;
  _data[2] = heatIndex;

  // save last data
  _lastTemperature = temperature;
  _lastHumidity = humidity;
  _lastHeatIndex = heatIndex;

  return _data;
}
