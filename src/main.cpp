/*
26.09.2016 by Admin :-)

EL/ROOM/BMP   - BMP data    {temperature;pressure;altitude}
EL/ROOM/DHT   - DHT data    {temperature;humidity;heatIndex}
EL/CPU/SYSTEM - system data {freeHeap;vcc}
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "http_manager.h"
#include "dht_manager.h"
#include "bmp_manager.h"

ADC_MODE(ADC_VCC);		// internal ADC

// DHT
#define DHT_PIN       13   // pin D7
#define DHT_TYPE      11   // DHT11
// BMP
#define BMP_SDA       12   // pin D6
#define BMP_SCL       14   // pin D5
#define BMP_ADDR      0x76 // I2C address
// WIFI and MQTT
#define SSID          "ThomsonAP"
#define PASSWORD      "zxcasdqwe"
#define MQTT_SERVER   "broker.hivemq.com"
#define MQTT_ID       "qwerty123456"

// intervals
const int amountOfIntervals                      = 2;
unsigned long previousMillis[amountOfIntervals]  = {0, 0};
unsigned long currentMillis                      = 0;
int intervals[amountOfIntervals]                 = {10000, 30000};

// functions
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();
void mqttPublish();
void httpPost();

// network
WiFiClient wifiClient;
PubSubClient mqttClient;
HTTPManager httpm;

// sensors
DHTManager dhtm(DHT_PIN, DHT_TYPE);
BMPManager bmpm(BMP_SDA, BMP_SCL, BMP_ADDR);

// structures
DHTManager::DHTData dhtData;
BMPManager::BMPData bmpData;

void setup() {
	// serial
	Serial.begin(115200);
	delay(50);

	// BMP
	if (!bmpm.begin()) {
		Serial.println("Could not find a valid BMP280 sensor, check wiring!");
		while (1);
	}

	// wifi
	Serial.print("\nConnecting to ");
	Serial.print(SSID);

	WiFi.begin(SSID, PASSWORD);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("\nWiFi connected.");
	Serial.print("IP address: ");
	Serial.print(WiFi.localIP());

	// mqtt
	mqttClient.setClient(wifiClient);
	mqttClient.setServer(MQTT_SERVER, 1883);
	mqttClient.setCallback(callback);
}

void loop() {
	if (!mqttClient.connected()) {
		reconnect();
	}
	mqttClient.loop();

	// get time and run intervals
	currentMillis = millis();
	for(int i=0; i < amountOfIntervals; i++) {
		if ((unsigned long)(currentMillis - previousMillis[i]) >= intervals[i]) {
			if(i==0) {
				mqttPublish();
			}
			if(i==1) {
				httpPost();
			}
			previousMillis[i] = currentMillis;
		}
	}
}

void reconnect() {
	while (!mqttClient.connected()) {
		Serial.print("\nAttempting MQTT connection... ");
		// Attempt to connect
		if (mqttClient.connect(MQTT_ID)) {
			Serial.println("connected");

			// publish

			// resubscribe
			mqttClient.subscribe("LAB/TEST/DATA");
		} else {
			Serial.print("\nfailed, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" try again in 5 seconds");
			// Wait 5 seconds before retrying
			delay(5000);
		}
	}
}

void callback(char *topic, byte *payload, unsigned int length) {
	// Serial.print("Message arrived [");
	// Serial.print(topic);
	// Serial.print("] ");
	// for (int i = 0; i < length; i++) {
	// 	Serial.print((char) payload[i]);
	// }
}

void mqttPublish() {
	// get DHT data and publish
	if (dhtm.getData(dhtData)) {
		String dht11 = String(dhtData.temperature, 2) + ";" +
									 String(dhtData.humidity, 2) + ";" +
									 String(dhtData.heatIndex, 2);
		mqttClient.publish("EL/ROOM/DHT", dht11.c_str());
	}

	// get BMP data and publish
	bmpm.getData(bmpData, true, 1021);		// true - pressure in mmHg
	String bmp280 = String(bmpData.temperature, 2) + ";" +
									String(bmpData.pressure, 2) + ";" +
									String(bmpData.altitude, 2);
	mqttClient.publish("EL/ROOM/BMP", bmp280.c_str());

	// get free heap and publish
	String sys = String(ESP.getFreeHeap()) + ";" + String(ESP.getVcc() / 1024.0, 2);
	mqttClient.publish("EL/CPU/SYSTEM", sys.c_str());
}

void httpPost() {
	// dht11
	if (dhtm.getData(dhtData)) {
		String dht11 = "temperature=" + String(dhtData.temperature, 2) +
								 	 "&humidity=" + String(dhtData.humidity, 2) +
								 	 "&heat_index=" + String(dhtData.heatIndex, 2);
		httpm.POST("http://iot-gkdevmaster.rhcloud.com/api/v1/dht11", dht11);
	}

	// bmp280
	bmpm.getData(bmpData, true, 1021);		// true - pressure in mmHg
	String bmp280 = "temperature=" + String(bmpData.temperature, 2) +
									"&pressure=" + String(bmpData.pressure, 2) +
									"&altitude=" + String(bmpData.altitude, 2);
	httpm.POST("http://iot-gkdevmaster.rhcloud.com/api/v1/bmp280", bmp280);

	// // system
	// String sys = "free_heap=" + String(ESP.getFreeHeap()) +
	// 						 "&vcc=" + String(ESP.getVcc() / 1024.0, 2);
	// httpm.POST("http://iot-gkdevmaster.rhcloud.com/api/v1/system", sys);
}
