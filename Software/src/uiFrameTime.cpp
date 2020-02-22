#include "uiGlobal.h"
#include "uiFrameTime.h"

#include <ESP8266WiFi.h>
#include <NTPClient.h>

void uiFrameTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    String timeText = String();

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);

    timeText = "Time: ";

    if ((WiFi.getMode() == WIFI_STA) && (WiFi.status() == WL_CONNECTED)) {

        (*(uiGlobalObject_s *)(state->userData)).ntpClient_p->update();
        timeText += (*(uiGlobalObject_s *)(state->userData)).ntpClient_p->getFormattedTime();
    } else {
        timeText += "Not Avail";
    }

    display->drawString(0 + x, 16 + y, timeText);
}
