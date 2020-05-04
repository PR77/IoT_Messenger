#include "uiGlobal.h"
#include <batteryHistogram.h>
#include "uiFrameBattery.h"

#include <ESP8266WiFi.h>

//=============================================================================
// Defines
//=============================================================================

#define BATTERY_AXIS_X_LENGTH           128
#define BATTERY_AXIS_Y_HEIGHT           20

//=============================================================================
// Voltage graph co-ordinates
//=============================================================================

/*
0,0                                                                       127,0
                                                   [RESERVED FOR FRAME OVERLAY]
| < (0,11)
|______________________________________________________________________________
0,31                                                                     127,31
*/

//=============================================================================
// Battery status monitor
//=============================================================================

void uiFrameBattery(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    uint16_t *batteryVoltageSamples = (*(uiGlobalObject_s *)(state->userData)).battery_p->GetBatteryHistogram();

    // draw axis'
    display->drawHorizontalLine(0 + x, 31 + y, BATTERY_AXIS_X_LENGTH);
    display->drawVerticalLine(0 + x, 11 + y, BATTERY_AXIS_Y_HEIGHT);

    // draw samples
    if (state->frameState == FIXED) {
        for (uint8_t i = 0; i < BATTERY_VOLTAGE_SAMPLES_MAX; i++) {

            // batteryVoltage will only plot voltages from 3 volts;
            // VBat = 3, when Y = 0, VBat = 4.42, when Y = 20.
            // Negative values not considered
            uint16_t sample = (uint16_t)(((batteryVoltageSamples[i] - 692) / 331.0) * 20.0);

            if (sample > 0) {
                // Only draw voltage if sample contains actual data
                display->drawLine(i, 31 - sample, i, 31);
            }
        }
    }
}
