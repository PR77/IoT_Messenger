#ifndef UI_FRAME_BATTERY
#define UI_FRAME_BATTERY

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Defines
//=============================================================================

#define BATTERY_WINDOW_SIZE_MINUTES     15
#define BATTERY_VOLTAGE_GRAPH_WIDTH     128 - 1
#define BATTERY_VOLTAGE_SAMPLES_MAX     BATTERY_VOLTAGE_GRAPH_WIDTH
#define BATTERY_SAMPLE_INTERVAL_SECONDS ((BATTERY_WINDOW_SIZE_MINUTES * 60) / BATTERY_VOLTAGE_GRAPH_WIDTH)
#define BATTERY_SAMPLE_INTERNAL_MS      BATTERY_SAMPLE_INTERVAL_SECONDS * 1000L

#define BATTERY_AXIS_X_LENGTH           128
#define BATTERY_AXIS_Y_HEIGHT           20

//=============================================================================
// Types
//=============================================================================

typedef struct {

    // batteryVoltageSamples are only uint8_t as they are only bar heights
    uint8_t batteryVoltageSamples[BATTERY_VOLTAGE_SAMPLES_MAX];
    uint8_t currentSample;
    uint32_t refreshTimer;
    bool initialised;

} batteryStatusObject_s;

//=============================================================================
// Prototypes
//=============================================================================

void uiFrameBattery(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

#endif // UI_FRAME_BATTERY
