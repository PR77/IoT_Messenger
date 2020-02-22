#ifndef UI_OVERLAY_H
#define UI_OVERLAY_H

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Prototypes
//=============================================================================

void uiOverlay(OLEDDisplay *display, OLEDDisplayUiState* state);

#endif // UI_OVERLAY_H
