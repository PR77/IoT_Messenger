#include "uiGlobal.h"
#include "uiFrameTunnel.h"

#include <ESP8266WiFi.h>

void uiFrameTunnel(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);
    display->drawString(64 + x, 16 + y, "...Time Tunnel...");
}
