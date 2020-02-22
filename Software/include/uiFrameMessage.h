#ifndef UI_FRAME_MESSAGE
#define UI_FRAME_MESSAGE

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Prototypes
//=============================================================================

void uiFrameMessage(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

#endif // UI_FRAME_MESSAGE
