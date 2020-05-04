#ifndef UI_GLOBAL
#define UI_GLOBAL

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

#include <NTPClient.h>
#include <DHTesp.h>

#include <batteryHistogram.h>

//=============================================================================
// Types
//=============================================================================

typedef enum {

    uiGlobalUXActive = 0,
    uiGlobalUXTetris,
    uiGlobalUXDino,
    uiGlobalUXTunnel,
    uiGlobalUXStars

} uiGlobalState_e;

typedef struct {

    String receivedMessage;
    bool newMessageReceived;
    bool messageRead;

} messageObject_s;

typedef struct {

    messageObject_s *messageObject_p;
    TempAndHumidity *dhtTempAndHumidity_p;
    NTPClient *ntpClient_p;
    BatteryHistogram *battery_p;

} uiGlobalObject_s;

//=============================================================================
// Externals
//=============================================================================

extern uiGlobalObject_s uiGlobalObject;

#endif // UI_GLOBAL
