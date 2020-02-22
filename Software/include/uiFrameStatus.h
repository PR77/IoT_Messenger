#ifndef UI_FRAME_STATUS
#define UI_FRAME_STATUS

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Prototypes
//=============================================================================

void uiFrameStatus(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

#endif // UI_FRAME_STATUS
