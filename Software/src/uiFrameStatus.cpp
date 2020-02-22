#include "uiFrameStatus.h"

#include <ESP8266WiFi.h>

void uiFrameStatus(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    String statusText = String();

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_10);

    statusText = "IP: " + WiFi.localIP().toString();
    statusText += ", " + String(WiFi.RSSI());
    display->drawString(0 + x, 11 + y, statusText);

    statusText = "FS: " + String(ESP.getFlashChipRealSize() / (1024*1024)) + "MB";
    statusText += ", FRQ: " + String(ESP.getCpuFreqMHz()) + "MHz";
    display->drawString(0 + x, 22 + y, statusText);
}
