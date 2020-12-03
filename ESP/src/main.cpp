#include <Arduino.h>
#include "WiFiHandler.h"
#include "Sensor.h"
#include "WebServerHandler.h"
#include "OTAHandler.h"

const char *hostname = "weather";
const char *otapw = "weather";
const char *ssid = "";
const char *password = "";

Sensor sensor;
WebServerHandler server;
OTAHandler ota;
WiFiHandler wifi;
String lastdata;

void setup()
{
  Serial.begin(9600);

  wifi.Initialize();
  ota.Initialize();
  sensor.Initialize();
  server.Initialize();
}

void loop()
{
  ota.Handle();

  server.GetWebSocketHandler()->CleanupClients();

  lastdata = sensor.Messure();
  server.GetWebSocketHandler()->SendAll(lastdata);

  delay(5000);
}