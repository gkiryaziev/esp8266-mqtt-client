#include "dht_manager.h"

DHTManager::DHTManager(int dhtPin, int dhtType) {
  dht = new DHT(dhtPin, dhtType);
  dht->begin();

  _lastTemperature = 0;
  _lastHumidity = 0;
  _lastHeatIndex = 0;
}

DHTManager::~DHTManager() {
  delete dht;
}

float *DHTManager::getData() {
  // get data
  float temperature = dht->readTemperature(false); // Celsius, true - Fahrenheit
  float humidity = dht->readHumidity();

  // check isnan
  if (isnan(temperature) || isnan(humidity)) {
    data[0] = _lastTemperature;
    data[1] = _lastHumidity;
    data[2] = _lastHeatIndex;
    return data;
  }

  // compute heat index
  float heatIndex = dht->computeHeatIndex(temperature, humidity, false); // Celsius

  data[0] = temperature;
  data[1] = humidity;
  data[2] = heatIndex;

  _lastTemperature = temperature;
  _lastHumidity = humidity;
  _lastHeatIndex = heatIndex;

  return data;
}
