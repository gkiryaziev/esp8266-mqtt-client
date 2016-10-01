
#include "bmp_manager.h"

BMPManager::BMPManager(int sda, int scl, uint8_t addr) {
  _addr = addr;
  Wire.begin(sda, scl);
  delay(50);
}

bool BMPManager::begin() {
  return _bmp.begin(_addr);
}

void *BMPManager::getData(BMPData &data, bool isPressureInMmHg, float seaLevelhPa) {

  // get data
  float temperature = _bmp.readTemperature();
  float pressure = 0.0;
  if (isPressureInMmHg) {
    pressure = _bmp.readPressure() / 133.322;
  } else {
    pressure = _bmp.readPressure();
  }
  float altitude = _bmp.readAltitude(seaLevelhPa);

  // save data
  data.temperature = temperature;
  data.pressure = pressure;
  data.altitude = altitude;
}
