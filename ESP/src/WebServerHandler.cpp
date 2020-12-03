#include "WebServerHandler.h"

void WebServerHandler::Initialize()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  m_ws = m_wsh.Initialize();
  m_server.addHandler(m_ws);

  m_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", String());
  });

  m_server.begin();
}

WebSocketHandler *WebServerHandler::GetWebSocketHandler()
{
  return &m_wsh;
}