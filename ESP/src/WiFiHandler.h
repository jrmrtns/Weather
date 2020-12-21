#include <WiFi.h>
#include "WeatherTypes.h"

extern const char *hostname;
extern const char *ssid;
extern const char *password;

class WiFiHandler
{
public:
  void Initialize()
  {
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
    WiFi.setHostname(hostname);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(200);
      Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());
  }
};