#include <ESP8266WiFi.h>

const char *SERVER_WIFI_SSID = "mywifi";
const char *SERVER_WIFI_PASS = "1111111111";

void setupWiFi()
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

time_t getCurrentTimeNTP(){
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(1000);
  }
  time_t now = time(nullptr);
  return now;
}

