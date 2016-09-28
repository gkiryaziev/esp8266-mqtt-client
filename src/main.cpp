/*
	26.09.2016 by Admin :-)

	EL/ROOM/DATA - DHT data		{temperature;humidity;heatIndex}
	EL/CPU/HEAP  - free heap
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "dht_manager.h"

// DHT
#define DHTPIN      2    // pin D4
#define DHTTYPE     11   // DHT11
// WIFI and MQTT
#define SSID        "ThomsonAP"
#define PASSWORD    "zxcasdqwe"
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_ID     "qwerty123456"
// system
#define INTERVAL    5000 // delay 5 sec.
#define BUFFER_SIZE 50  // message buffer

// variables
long lastMsg = 0;
char buffer[BUFFER_SIZE];

// functions
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

// objects
WiFiClient wifiClient;
PubSubClient mqttClient;
DHTManager dhtm(DHTPIN, DHTTYPE);

// structures
DHTManager::DHTData dhtData;

void setup() {
	// serial
	Serial.begin(115200);
	delay(50);

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

	// get data and publish
	long now = millis();
	if (now - lastMsg > INTERVAL) {
		lastMsg = now;

		// get DHT data and publish
		dhtm.getData(dhtData);
		snprintf(buffer, BUFFER_SIZE, "%s;%s;%s",
	    String(dhtData.temperature, 2).c_str(),
	    String(dhtData.humidity, 2).c_str(),
	    String(dhtData.heatIndex, 2).c_str()
	  );
		mqttClient.publish("EL/ROOM/DATA", buffer);

		// get free heap and publish
		snprintf(buffer, 50, "%d", ESP.getFreeHeap());
		mqttClient.publish("EL/CPU/HEAP", buffer);
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
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char) payload[i]);
	}
}
