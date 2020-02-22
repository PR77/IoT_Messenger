#ifndef UI_FRAME_TETRIS
#define UI_FRAME_TETRIS

#include "Arduino.h"

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Prototypes
//=============================================================================

void uiFrameTetris(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y);

#endif // UI_FRAME_TETRIS
