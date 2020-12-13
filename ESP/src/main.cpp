#include <Arduino.h>
#include "WiFiHandler.h"
#include "Sensor.h"
#include "WebServerHandler.h"
#include "OTAHandler.h"
#include <PID_v1.h>

const char *hostname = "weather";
const char *otapw = "weather";
const char *ssid = "";
const char *password = "";
sensorData lastdata;

Sensor sensor;
WebServerHandler server;
OTAHandler ota;
WiFiHandler wifi;
double output;
PID pid(&lastdata.temperature, &output, &lastdata.target, 2, 5, 1, DIRECT);
const byte heating_gpio = 32; // the PWM pin the heating is attached to

void setup()
{
  Serial.begin(9600);

  wifi.Initialize();
  ota.Initialize();
  sensor.Initialize();
  server.Initialize();
  lastdata.target = (double)(28.0);
  
  ledcAttachPin(heating_gpio, 0); // assign a pwm pin to a channel

  // https://techexplorations.com/guides/esp32/begin/pwm/
  // Initialize channels
  // channels 0-15, resolution 1-16 bits, freq limits depend on resolution
  // ledcSetup(uint8_t channel, uint32_t freq, uint8_t resolution_bits);
  ledcSetup(0, 4000, 8); 

  pid.SetMode(AUTOMATIC);
}

void loop()
{
  ota.Handle();

  server.GetWebSocketHandler()->CleanupClients();

  lastdata = sensor.Messure();
  server.GetWebSocketHandler()->SendAll(Sensor::ConvertToJson(lastdata));

  pid.Compute();
  ledcWrite(0, output);

  Serial.printf("Input: %f ", lastdata.temperature);
  Serial.printf("Target: %f ", lastdata.target);
  Serial.printf("Output: %f\r\n", output);

  delay(5000);
}