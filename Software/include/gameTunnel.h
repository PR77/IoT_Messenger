#ifndef GAME_TUNNEL
#define GAME_TUNNEL

#include "Arduino.h"

#include <clickButton.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Defines
//=============================================================================

#define TUNNEL_GAME_REFRESH 33  // 30 FPS

#define TEXTURE_WIDTH       8
#define TEXTURE_HEIGHT      8

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       32

//=============================================================================
// Types
//=============================================================================

typedef enum {

    gameTunnelStateIdle = 0,
    gameTunnelStateRunning,
    gameTunnelStateExit

} gameTunnelState_e;

typedef struct {

    gameTunnelState_e gameTunnelState;
    uint32_t refreshTimer;
    uint8_t animationStep;

} gameTunnelObject_s;

//=============================================================================
// Prototypes
//=============================================================================

gameTunnelState_e gameTunnelCyclic(OLEDDisplay *display, ClickButton *exit, uint16_t *gameRemainingBudget);

#endif // GAME_TUNNEL
