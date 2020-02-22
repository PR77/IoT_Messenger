#include "gameDino.h"

//=============================================================================
// Dino sprite
//=============================================================================

const uint8_t dinoSprite[] PROGMEM = {

    // https://www.dcode.fr/binary-image - LSB needs to be first in image data

    B00000000, B00111110,
    B00000000, B01111101,
    B00000000, B01111111,
    B00000000, B01111111,
    B00000000, B00011111,
    B10000010, B00000011,
    B11000010, B00000111,
    B11100110, B00001011,
    B11111110, B00000011,
    B11111100, B00000011,
    B11111000, B00000001,
    B11110000, B00000000,
    B00100000, B00000000
};

#define DINO_FOOT_X_POSITION    5

//=============================================================================
// Cloud sprite
//=============================================================================

const uint8_t cloudSprite1[] PROGMEM = {

    B10000000, B00000001,
    B01000000, B00000010,
    B10000000, B00000001,
    B00000000, B00000000,
    B00000000, B00000000
};

const uint8_t cloudSprite2[] PROGMEM = {

    B10000000, B00000011,
    B01100000, B00000100,
    B00011100, B00001000,
    B01111000, B00000111,
    B00000000, B00000000
};

const uint8_t cloudSprite3[] PROGMEM = {

    B10000000, B00000011,
    B01100000, B00000100,
    B00011100, B00111000,
    B00000010, B01000000,
    B11111100, B01111111
};

const uint8_t cloudSprite4[] PROGMEM = {

    B10000000, B00000011,
    B01100000, B00000100,
    B00011100, B00111000,
    B00000010, B01000000,
    B11111100, B01111111
};

const uint8_t * cloudSprites[CLOUD_SPRITE_COUNT] PROGMEM = {
    cloudSprite1,
    cloudSprite2,
    cloudSprite3,
    cloudSprite4
};

//=============================================================================
// Global object
//=============================================================================

gameDinoObject_s gameDinoObject;

//=============================================================================
// Dino game global object initialisation
//=============================================================================

void gameDinoInitialise(void) {

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        gameDinoObject.dinoObstacle[i].height = 4;
        gameDinoObject.dinoObstacle[i].width = 5;
        gameDinoObject.dinoObstacle[i].speed = 4;
        gameDinoObject.dinoObstacle[i].xPosition = (DINO_SPRITE_WIDTH + DINO_SPRITE_WIDTH);
        gameDinoObject.dinoObstacle[i].xPosition += rand() % (127 - (DINO_SPRITE_WIDTH + DINO_SPRITE_WIDTH));
        gameDinoObject.dinoObstacle[i].yPosition = 31;
    }

    gameDinoObject.obstacleRefreshDelay = OBSTACLE_REFRESH_DELAY;

    for (uint8_t i = 0; i < MAX_DIRT_COUNT; i++) {
        gameDinoObject.dirtPixels[i] = rand() % 127;
    }

    gameDinoObject.dinoScore = 0;
    gameDinoObject.dinoGravity = 1;
    gameDinoObject.dinoYPosition = 31 - DINO_SPRITE_HEIGHT;

    // Start at right of screen, considering score field of 10pt.
    gameDinoObject.cloudState = cloudForming;
    gameDinoObject.cloudSprintIndex = 0;
    gameDinoObject.cloudXPosition = rand() % (127 - CLOUD_SPRITE_WIDTH - 10);
    gameDinoObject.cloudRefreshDelay = CLOUD_REFRESH_DELAY;

    gameDinoObject.dinoJumpDirection = jumpingUp;
    gameDinoObject.jumpPressed = false;
    gameDinoObject.dinoJumping = false;

    gameDinoObject.gameOverState = displayFlashing;
    gameDinoObject.gameOverRefreshDelay = GAME_OVER_REFRESH_DELAY;
    gameDinoObject.gameOverFlashCount = 0;
}

//=============================================================================
// Dino game
//=============================================================================

void gameDinoDrawGround(OLEDDisplay *display) {

    display->clear();
    display->drawLine(0, 31, 127, 31);

    for (uint8_t i = 0; i < MAX_DIRT_COUNT; i++) {
        display->clearPixel(gameDinoObject.dirtPixels[i], 31);
    }
}

void gameDinoDrawClouds(OLEDDisplay *display) {
    uint8_t cloudXPosition;

    if (gameDinoObject.cloudState == cloudForming) {

        if (!gameDinoObject.cloudRefreshDelay) {
            gameDinoObject.cloudRefreshDelay = CLOUD_REFRESH_DELAY;

            if (gameDinoObject.cloudSprintIndex < CLOUD_SPRITE_COMPLETE_INDEX) {
                gameDinoObject.cloudSprintIndex++;
            } else {
                gameDinoObject.cloudState = cloudScrolling;
            }
        }
        else {
            gameDinoObject.cloudRefreshDelay -= CLOUD_SCROLL_STEP;
        }
    }

    if (gameDinoObject.cloudState == cloudScrolling) {
        if (!gameDinoObject.cloudRefreshDelay) {
            gameDinoObject.cloudRefreshDelay = CLOUD_REFRESH_DELAY;

            if ((gameDinoObject.cloudXPosition - CLOUD_SCROLL_STEP) > 0) {
                gameDinoObject.cloudXPosition -= CLOUD_SCROLL_STEP;

                // Count to index.
                gameDinoObject.cloudSprintIndex = CLOUD_SPRITE_COMPLETE_INDEX;
            } else {
                gameDinoObject.cloudXPosition = rand() % (127 - CLOUD_SPRITE_WIDTH - 10);
                gameDinoObject.cloudState = cloudForming;
                gameDinoObject.cloudSprintIndex = 0;
            }
        } else {
            gameDinoObject.cloudRefreshDelay -= CLOUD_SCROLL_STEP;
        }
    }

    cloudXPosition = gameDinoObject.cloudXPosition;

    display->drawXbm(cloudXPosition, CLOUD_SPRITE_HEIGHT, CLOUD_SPRITE_WIDTH,
        CLOUD_SPRITE_HEIGHT, cloudSprites[gameDinoObject.cloudSprintIndex]);
}

void gameDinoDrawObstacles(OLEDDisplay *display) {

    if (!gameDinoObject.obstacleRefreshDelay) {
        gameDinoObject.obstacleRefreshDelay = OBSTACLE_REFRESH_DELAY;

        for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
            if ((gameDinoObject.dinoObstacle[i].xPosition - gameDinoObject.dinoObstacle[i].speed) > 0) {
                gameDinoObject.dinoObstacle[i].xPosition -= gameDinoObject.dinoObstacle[i].speed;
            } else {
                gameDinoObject.dinoObstacle[i].xPosition = (DINO_SPRITE_WIDTH + DINO_SPRITE_WIDTH);
                gameDinoObject.dinoObstacle[i].xPosition += rand() % (127 - (DINO_SPRITE_WIDTH + DINO_SPRITE_WIDTH));

                // If obstacle had to be redrawn, then there was no collision so bump up score.
                gameDinoObject.dinoScore++;
            }
        }
    } else {
        gameDinoObject.obstacleRefreshDelay -= OBSTACLE_SCROLL_STEP;
    }

    // Always refresh obstacle positions regardless of position updates.

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        int16_t x = gameDinoObject.dinoObstacle[i].xPosition;
        int16_t y = gameDinoObject.dinoObstacle[i].yPosition;
        int16_t height = gameDinoObject.dinoObstacle[i].height;
        int16_t width = gameDinoObject.dinoObstacle[i].width;

        display->fillRect(x, y - height, width, height);
    }
}

void gameDinoDrawDino(OLEDDisplay *display) {
    uint8_t dinoYPosition;

    if (gameDinoObject.jumpPressed == true) {
        gameDinoObject.dinoJumpDirection = jumpingUp;
        gameDinoObject.dinoJumping = true;

        gameDinoObject.jumpPressed = false;
    }

    if (gameDinoObject.dinoJumping == true) {

        if (gameDinoObject.dinoJumpDirection == jumpingUp) {
            if ((gameDinoObject.dinoYPosition - gameDinoObject.dinoGravity) > 0) {
                gameDinoObject.dinoYPosition -= gameDinoObject.dinoGravity;
            } else {
                gameDinoObject.dinoJumpDirection = comingDown;
            }
        }

        // Else statement used here to ensure Dino jumps directly to top of
        // screen. Having the If == comingDown is used then line 0 is skipped.
        else {
            if ((gameDinoObject.dinoYPosition + gameDinoObject.dinoGravity) <= (31 - DINO_SPRITE_HEIGHT)) {
                gameDinoObject.dinoYPosition += gameDinoObject.dinoGravity;
            } else {
                gameDinoObject.dinoJumping = false;
            }
        }
    }

    dinoYPosition = gameDinoObject.dinoYPosition;

    display->drawXbm(0, dinoYPosition, DINO_SPRITE_WIDTH, DINO_SPRITE_HEIGHT, dinoSprite);
}

void gameDinoDrawScore(OLEDDisplay *display) {
    display->setFont(ArialMT_Plain_10);
    display->setTextAlignment(TEXT_ALIGN_RIGHT);
    display->drawString(128, 0, String(gameDinoObject.dinoScore));
}

gameDinoCollision_e gameDinoCheckCollision(void) {
    gameDinoCollision_e gameDinoCollision = noCollision;
    uint8_t closestObstacleXPosition = 0;
    uint8_t closestObstacleHeight = 0;

    closestObstacleXPosition = gameDinoObject.dinoObstacle[0].xPosition;
    closestObstacleHeight = gameDinoObject.dinoObstacle[0].height;

    for (uint8_t i = 0; i < MAX_OBSTACLES; i++) {
        if (gameDinoObject.dinoObstacle[i].xPosition < closestObstacleXPosition) {
            closestObstacleXPosition = gameDinoObject.dinoObstacle[i].xPosition;
            closestObstacleHeight = gameDinoObject.dinoObstacle[i].height;
        }
    }

    if (closestObstacleXPosition <= DINO_FOOT_X_POSITION) {
        // X cordinate matches dino's foot. Now check Y cordinate.

        if (gameDinoObject.dinoYPosition > (DINO_SPRITE_HEIGHT + closestObstacleHeight)) {
            gameDinoCollision = collisionWithObstacle;
        }
    }

    return (gameDinoCollision);
}

void gameDinoDrawGameOver(OLEDDisplay *display) {

    if (!gameDinoObject.gameOverRefreshDelay) {
        gameDinoObject.gameOverRefreshDelay = GAME_OVER_REFRESH_DELAY;

        if (gameDinoObject.gameOverState == displayFlashing) {
            // Invert, Normal, Invert, Normal, Invert, Normal

            if (gameDinoObject.gameOverFlashCount % 2) {
                // on odd numbers invert display
                display->invertDisplay();
            } else {
                display->normalDisplay();
            }

            if (gameDinoObject.gameOverFlashCount < GAME_OVER_FLASH_COUNT) {
                gameDinoObject.gameOverFlashCount += GAME_OVER_FLASH_STEP;
            } else {
                gameDinoObject.gameOverState = displayScore;
            }
        }

        if (gameDinoObject.gameOverState == displayScore) {
            display->clear();
            display->setFont(ArialMT_Plain_16);
            display->setTextAlignment(TEXT_ALIGN_CENTER);
            display->drawString(64, 0, "...Game Over...");
            display->setFont(ArialMT_Plain_10);
            display->drawString(64, 21, String(gameDinoObject.dinoScore));
        }

    } else {
        gameDinoObject.gameOverRefreshDelay -= GAME_OVER_FLASH_STEP;
    }
}

gameDinoState_e gameDinoCyclic(OLEDDisplay *display, ClickButton *jump, ClickButton *exit, uint16_t *gameRemainingBudget) {
    uint32_t currentTime = millis();

    if (gameDinoObject.gameDinoState == gameDinoStateIdle) {

        gameDinoInitialise();

        gameDinoObject.refreshTimer = 0;
        gameDinoObject.gameDinoState = gameDinoStateRunning;
    }

    if ((currentTime - gameDinoObject.refreshTimer) >=  DINO_GAME_REFRESH) {

        gameDinoObject.refreshTimer = currentTime;

        // Check for collision first so frame is correctly drawn from previous.
        if (gameDinoCheckCollision() == collisionWithObstacle) {
            gameDinoObject.gameDinoState = gameDinoStateGameOver;
        }

        if (gameDinoObject.gameDinoState == gameDinoStateRunning) {
            gameDinoDrawGround(display);
            gameDinoDrawClouds(display);
            gameDinoDrawObstacles(display);
            gameDinoDrawDino(display);
            gameDinoDrawScore(display);
        }

        if (gameDinoObject.gameDinoState == gameDinoStateGameOver) {
            gameDinoDrawGameOver(display);
        }

        display->display();
    }

    *gameRemainingBudget = DINO_GAME_REFRESH - (millis() - currentTime);

    if (jump->clicks > 0) {
        gameDinoObject.jumpPressed = true;
    }

    if (exit->clicks > 0) {
        gameDinoObject.gameDinoState = gameDinoStateIdle;
        return (gameDinoStateExit);
    }

    return (gameDinoObject.gameDinoState);
}
