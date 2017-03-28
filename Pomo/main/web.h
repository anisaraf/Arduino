#ifndef WEB_H
#define WEB_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

namespace web {
  const char *SERVER_WIFI_SSID = "mywifi";
  const char *SERVER_WIFI_PASS = "1111111111";
  time_t start_time = 0; // time when program started in seconds
  int timezone = -5; // EST
  int dst_offset = 1; // Automate at some point

  // Connect to the internet with provided creds.
  void ConnectWiFi()
  {
    Serial.print("Connecting to WiFi ");
    WiFi.begin(SERVER_WIFI_SSID,SERVER_WIFI_PASS);
    while(WiFi.status() != WL_CONNECTED)
      {
	delay(500);
	Serial.print(".");
      }
 
    Serial.println("Connected");
  }

  // Set up handlers for OTA updates. Must add ArduinoOTA.handle(); in loop()
  void SetupOTAHandlers() {
    ConnectWiFi();
    ArduinoOTA.onStart([]() {
	String type;
	if (ArduinoOTA.getCommand() == U_FLASH)
	  type = "sketch";
	else // U_SPIFFS
	  type = "filesystem";

	// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
	Serial.println("Start updating " + type);
      });
    ArduinoOTA.onEnd([]() {
	Serial.println("\nEnd");
      });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
	Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      });
    ArduinoOTA.onError([](ota_error_t error) {
	Serial.printf("Error[%u]: ", error);
	if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
	else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
	else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
	else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
	else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  void CheckOTAUpdates() {
    ArduinoOTA.handle();
  }

  // Set actual program start time from NTP servers.
  void setStartTimeNTP(){
    configTime(timezone * 3600, 0, "pool.ntp.org", "time.nist.gov");
    Serial.print("Getting time from server.");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }
    time_t now = time(nullptr);
    start_time = now - millis()/1000 + dst_offset * 3600;
  }

  
}


#endif
