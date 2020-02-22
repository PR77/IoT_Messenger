#ifndef UI_FRAME_TIME
#define UI_FRAME_TIME

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Prototypes
//=============================================================================

void uiFrameTime(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

#endif // UI_FRAME_TIME
