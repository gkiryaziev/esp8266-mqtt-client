##	IoT MQTT Client

This MQTT client written for [ESP8266 NodeMCU](http://nodemcu.com/index_en.html) board with [Arduino core](https://github.com/esp8266/Arduino) in [PlatformIO IDE](http://platformio.org/).

## Library Dependency

- 1. [PubSubClient](http://pubsubclient.knolleary.net/)
- 2. [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)
- 3. [Adafruit BMP280 Library](https://github.com/adafruit/Adafruit_BMP280_Library)
- 4. [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)

## Topics
```
EL/ROOM/BMP - BMP data    {temperature;pressure;altitude}
EL/ROOM/DHT - DHT data    {temperature;humidity;heatIndex}
EL/CPU/HEAP - free heap
```
