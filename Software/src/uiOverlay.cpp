#include "uiGlobal.h"
#include "uiOverlay.h"

#include <ESP8266WiFi.h>

const uint8_t newMessage[] PROGMEM = {

    B11111111,
    B11000011,
    B10100101,
    B10011001,
    B10000001,
    B10000001,
    B11111111,
};

void uiOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
    String overlayText = String();

    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->setFont(ArialMT_Plain_10);

    overlayText = "vBatt: ";
    overlayText += ((analogRead(A0) / 1023.0) * 4.43);
    overlayText += "V, UiFrm: ";
    overlayText += state->currentFrame;

    display->drawString(128, 0, overlayText);

    if ((*(uiGlobalObject_s *)(state->userData)).messageObject_p->newMessageReceived == true) {
        display->drawXbm(0, 2, 8, 7, newMessage);
    }
}
