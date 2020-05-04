#include "gameStars.h"

//=============================================================================
// Global object
//=============================================================================

gameStarsObject_s gameStarsObject;
starObject_s stars[NUMBER_OF_STARS];

//=============================================================================
// Stars game global object initialisation
//=============================================================================

void gameStarsInitialise(void) {

    for (uint8_t i = 0; i < NUMBER_OF_STARS; i++) {

        stars[i].speed = random(1, 4);
        stars[i].xPosition = rand() % SCREEN_WIDTH;
        stars[i].yPosition = rand() % SCREEN_HEIGHT;
    }
}

//=============================================================================
// Stars game
//=============================================================================

gameStarsState_e gameStarsCyclic(OLEDDisplay *display, ClickButton *exit, uint16_t *gameRemainingBudget) {
    uint32_t currentTime = millis();

    if (gameStarsObject.gameStarsState == gameStarsStateIdle) {

        gameStarsInitialise();

        gameStarsObject.refreshTimer = 0;
        gameStarsObject.gameStarsState = gameStarsStateRunning;
    }

    if ((gameStarsObject.gameStarsState == gameStarsStateRunning) &&
        (currentTime - gameStarsObject.refreshTimer) >= STARS_GAME_REFRESH) {

        display->clear();

        for (uint8_t i = 0; i < NUMBER_OF_STARS; i++) {
            uint8_t xPosition = stars[i].xPosition;
            uint8_t yPosition = stars[i].yPosition;

            xPosition -= stars[i].speed;

            if (xPosition == 0) {
                xPosition = rand() % SCREEN_WIDTH;
            }

            stars[i].xPosition = xPosition;

            display->setPixel(xPosition, yPosition);
        }

        display->display();
    }

    *gameRemainingBudget = STARS_GAME_REFRESH - (millis() - currentTime);

    if (exit->clicks > 0) {
        gameStarsObject.gameStarsState = gameStarsStateIdle;
        return (gameStarsStateExit);
    }

    return (gameStarsObject.gameStarsState);
}
