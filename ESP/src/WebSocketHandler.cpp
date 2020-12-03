#include "WebSocketHandler.h"

AsyncWebSocket* WebSocketHandler::Initialize()
{
  m_ws.onEvent(onEvent);
  return &m_ws;
}

void WebSocketHandler::CleanupClients()
{
  m_ws.cleanupClients();
}

void WebSocketHandler::SendAll(String message)
{
  m_ws.textAll(message);
}