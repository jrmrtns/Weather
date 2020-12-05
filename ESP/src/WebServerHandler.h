#pragma once

#include <Arduino.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include "ArduinoJson.h"
#include "WebSocketHandler.h"

class WebServerHandler
{
private:
  AsyncWebServer m_server = AsyncWebServer(80);
  AsyncWebSocket *m_ws;
  WebSocketHandler m_wsh;

public:
  void Initialize();
  WebSocketHandler *GetWebSocketHandler();
};
