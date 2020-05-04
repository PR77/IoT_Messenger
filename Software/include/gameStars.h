#ifndef GAME_STARS
#define GAME_STARS

#include "Arduino.h"

#include <clickButton.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Defines
//=============================================================================

#define STARS_GAME_REFRESH  33  // 30 FPS

#define NUMBER_OF_STARS     32

#define SCREEN_WIDTH        128
#define SCREEN_HEIGHT       32

//=============================================================================
// Types
//=============================================================================

typedef enum {

    gameStarsStateIdle = 0,
    gameStarsStateRunning,
    gameStarsStateExit

} gameStarsState_e;

typedef struct {

    uint8_t speed;
    uint8_t xPosition;
    uint8_t yPosition;

} starObject_s;

typedef struct {

    gameStarsState_e gameStarsState;
    uint32_t refreshTimer;

} gameStarsObject_s;

//=============================================================================
// Prototypes
//=============================================================================

gameStarsState_e gameStarsCyclic(OLEDDisplay *display, ClickButton *exit, uint16_t *gameRemainingBudget);

#endif // GAME_STARS
