#include "uiFrameBattery.h"

#include <ESP8266WiFi.h>

//=============================================================================
// Global object
//=============================================================================

batteryStatusObject_s batteryStatusObject;

//=============================================================================
// Battery status monitor
//=============================================================================

void uiFrameBattery(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    uint32_t currentTime = millis();

    if (batteryStatusObject.initialised == false) {
        for (uint8_t i = 0; i < BATTERY_VOLTAGE_SAMPLES_MAX; i++) {
            batteryStatusObject.batteryVoltageSamples[i] = 0;
        }

        // set refreshTimer to currentTime to ensure delay when frame shown
        batteryStatusObject.refreshTimer = currentTime;
        batteryStatusObject.currentSample = 0;
        batteryStatusObject.initialised = true;
    }

    if ((currentTime - batteryStatusObject.refreshTimer) >=  BATTERY_SAMPLE_INTERNAL_MS) {
        uint8_t sampleIndex = batteryStatusObject.currentSample;

        // batteryVoltage will only plot voltages from 3 volts;
        // VBat = 3, when Y = 0, VBat = 4.42, when Y = 20.
        // Negative values not considered
        uint8_t batteryVoltage = (uint8_t)(((analogRead(A0) - 692) / 331.0) * 20.0);

        batteryStatusObject.refreshTimer = currentTime;

        // stuff a new sample into the sample array
        batteryStatusObject.batteryVoltageSamples[sampleIndex] = batteryVoltage;

        if (sampleIndex == BATTERY_VOLTAGE_SAMPLES_MAX) {
            // sample array full, so now make it a sliding window array

            for (uint8_t i = 0; i < BATTERY_VOLTAGE_SAMPLES_MAX; i++) {
                batteryStatusObject.batteryVoltageSamples[i] = batteryStatusObject.batteryVoltageSamples[i + 1];
            }
        } else {
            sampleIndex++;
        }

        batteryStatusObject.currentSample = sampleIndex;
    }

    // draw axis'
    display->drawHorizontalLine(0 + x, 31 + y, BATTERY_AXIS_X_LENGTH);
    display->drawVerticalLine(0 + x, 11 + y, BATTERY_AXIS_Y_HEIGHT);

    // draw samples
    if (state->frameState == FIXED) {
        for (uint8_t i = 0; i < BATTERY_VOLTAGE_SAMPLES_MAX; i++) {
            display->drawLine(i + 1, 31, i + 1, 31 - batteryStatusObject.batteryVoltageSamples[i]);
        }
    }
}
