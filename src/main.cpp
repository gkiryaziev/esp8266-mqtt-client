/*
	26.09.2016 by Admin :-)

	EL/ROOM/DATA - DHT data		{temperature;humidity;heatIndex}
	EL/CPU/HEAP  - free heap
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "dht_manager.h"

// variables
const char *ssid       = "ThomsonAP";
const char *password   = "zxcasdqwe";
const char *mqttServer = "test.mosquitto.org";
const char *mqttID     = "qwerty123456";
long lastMsg           = 0;
int interval           = 5000; // 5 sec.

char buffer[50];       // message

// functions
void callback(char *topic, byte *payload, unsigned int length);
void reconnect();

// objects
WiFiClient wifiClient;
PubSubClient mqttClient;
DHTManager *dhtm;

void setup() {
	// serial
	Serial.begin(115200);
	delay(50);

	// wifi
	Serial.print("\nConnecting to ");
	Serial.print(ssid);

	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("\nWiFi connected.");
	Serial.print("IP address: ");
	Serial.print(WiFi.localIP());

	// mqtt
	mqttClient.setClient(wifiClient);
	mqttClient.setServer(mqttServer, 1883);
	mqttClient.setCallback(callback);

	// DHT manager
	dhtm = new DHTManager(2, 11); // D4, DHT11
}

void loop() {
	if (!mqttClient.connected()) {
		reconnect();
	}
	mqttClient.loop();

	// get data and publish
	long now = millis();
	if (now - lastMsg > interval) {
		lastMsg = now;

		// get DHT data and publish
		float *data = dhtm->getData();
		snprintf(buffer, 50, "%s;%s;%s",
	    String(data[0], 2).c_str(),
	    String(data[1], 2).c_str(),
	    String(data[2], 2).c_str()
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
		if (mqttClient.connect(mqttID)) {
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
