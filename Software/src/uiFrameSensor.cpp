#include "uiGlobal.h"
#include "uiFrameSensor.h"

#include <ESP8266WiFi.h>
#include <DHTesp.h>

void uiFrameSensor(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    String sensorText = String();

    display->setTextAlignment(TEXT_ALIGN_LEFT);
    display->setFont(ArialMT_Plain_16);

    sensorText = "T: ";
    sensorText += (*(uiGlobalObject_s *)(state->userData)).dhtTempAndHumidity_p->temperature;
    sensorText += ", H: ";
    sensorText += (*(uiGlobalObject_s *)(state->userData)).dhtTempAndHumidity_p->humidity;
    display->drawString(0 + x, 16 + y, sensorText);
}
