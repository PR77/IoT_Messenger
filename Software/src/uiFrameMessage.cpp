#include "uiGlobal.h"
#include "uiFrameMessage.h"

#include <ESP8266WiFi.h>

void uiFrameMessage(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    String messageText = String();

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);

    messageText = (*(uiGlobalObject_s *)(state->userData)).messageObject_p->receivedMessage;
    display->drawString(0 + x, 16 + y, messageText);

    (*(uiGlobalObject_s *)(state->userData)).messageObject_p->messageRead = true;
    (*(uiGlobalObject_s *)(state->userData)).messageObject_p->newMessageReceived = false;
}
