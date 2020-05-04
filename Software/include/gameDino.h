#ifndef GAME_DINO
#define GAME_DINO

#include "Arduino.h"

#include <clickButton.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>

//=============================================================================
// Defines
//=============================================================================

#define DINO_GAME_REFRESH               33  // 30 FPS

#define DINO_SPRITE_WIDTH               16
#define DINO_SPRITE_HEIGHT              13

#define CLOUD_SPRITE_COUNT              4
#define CLOUD_SPRITE_COMPLETE_INDEX     (CLOUD_SPRITE_COUNT - 1)
#define CLOUD_SPRITE_WIDTH              16
#define CLOUD_SPRITE_HEIGHT             5
#define CLOUD_SCROLL_STEP               1
#define CLOUD_REFRESH_DELAY             3

#define MAX_DIRT_COUNT                  3
#define MAX_OBSTACLES                   1
#define OBSTACLE_SCROLL_STEP            1
#define OBSTACLE_REFRESH_DELAY          3

#define GAME_OVER_FLASH_COUNT           6
#define GAME_OVER_FLASH_STEP            1
#define GAME_OVER_REFRESH_DELAY         3

//=============================================================================
// Types
//=============================================================================

typedef enum {

    gameDinoStateIdle = 0,
    gameDinoStateRunning,
    gameDinoStateGameOver,
    gameDinoStateExit

} gameDinoState_e;

typedef enum {

    noCollision = 0,
    collisionWithObstacle

} gameDinoCollision_e;

typedef enum {

    displayFlashing = 0,
    displayScore

} gameDinoGameOver_e;

typedef enum {

    jumpingUp = 0,
    comingDown

} gameDinoJumpDirection_e;

typedef enum {

    cloudForming = 0,
    cloudScrolling

} gameDinoCloudState_e;

typedef struct {

    uint8_t width;
    uint8_t height;
    uint8_t speed;
    uint8_t xPosition;
    uint8_t yPosition;

} gameDinoObstacle_s;

typedef struct {

    gameDinoState_e gameDinoState;
    uint16_t dinoScore;
    gameDinoObstacle_s dinoObstacle[MAX_OBSTACLES];
    uint8_t obstacleRefreshDelay;
    uint8_t dirtPixels[MAX_DIRT_COUNT];
    uint8_t dinoGravity;
    uint32_t refreshTimer;
    uint8_t dinoYPosition;
    uint8_t cloudXPosition;
    gameDinoCloudState_e cloudState;
    uint8_t cloudSprintIndex;
    uint8_t cloudRefreshDelay;
    gameDinoJumpDirection_e dinoJumpDirection;
    bool jumpPressed;
    bool dinoJumping;
    gameDinoGameOver_e gameOverState;
    uint8_t gameOverRefreshDelay;
    uint8_t gameOverFlashCount;

} gameDinoObject_s;

//=============================================================================
// Prototypes
//=============================================================================

gameDinoState_e gameDinoCyclic(OLEDDisplay *display, ClickButton *jump, ClickButton *exit, uint16_t *gameRemainingBudget);

#endif // GAME_DINO
