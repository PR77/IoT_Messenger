#include "uiGlobal.h"
#include "uiFrameStars.h"

#include <ESP8266WiFi.h>

void uiFrameStars(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->setTextAlignment(TEXT_ALIGN_CENTER);
    display->setFont(ArialMT_Plain_16);
    display->drawString(64 + x, 16 + y, "...To The Stars...");
}
