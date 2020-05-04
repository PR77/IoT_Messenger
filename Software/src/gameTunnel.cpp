#include "gameTunnel.h"

//=============================================================================
// Global object
//=============================================================================

gameTunnelObject_s gameTunnelObject;

uint8_t textureMap[TEXTURE_HEIGHT][TEXTURE_WIDTH];
uint8_t distanceTable[SCREEN_HEIGHT][SCREEN_WIDTH];
uint8_t angleTable[SCREEN_HEIGHT][SCREEN_WIDTH];

//=============================================================================
// Tunnel game global object initialisation
//=============================================================================

void gameTunnelInitialise(void) {

    for (uint8_t y = 0; y < TEXTURE_HEIGHT; y+=2) {
        for (uint8_t x = 0; x < TEXTURE_WIDTH; x+=2) {
            if ((x + y) % 2) {
                textureMap[y][x] = 0;
            } else {
                textureMap[y][x] = 1;
            }
        }
    }

    for(uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
        for(uint8_t x = 0; x < SCREEN_WIDTH; x++) {
            int angle, distance;
            float ratio = 4.0;

            distance = (int8_t)(ratio * TEXTURE_HEIGHT / sqrt((x - SCREEN_WIDTH / 2.0) * (x - SCREEN_WIDTH / 2.0) + (y - SCREEN_HEIGHT / 2.0) * (y - SCREEN_HEIGHT / 2.0))) % TEXTURE_HEIGHT;
            angle = (int8_t)(0.5 * TEXTURE_WIDTH * atan2(y - SCREEN_HEIGHT / 2.0, x - SCREEN_WIDTH / 2.0) / 3.1416);
            distanceTable[y][x] = distance;
            angleTable[y][x] = angle;
        }
    }
}

//=============================================================================
// Tunnel game
//=============================================================================

gameTunnelState_e gameTunnelCyclic(OLEDDisplay *display, ClickButton *exit, uint16_t *gameRemainingBudget) {
    uint32_t currentTime = millis();

    if (gameTunnelObject.gameTunnelState == gameTunnelStateIdle) {

        gameTunnelInitialise();

        gameTunnelObject.refreshTimer = 0;
        gameTunnelObject.animationStep = 0;
        gameTunnelObject.gameTunnelState = gameTunnelStateRunning;
    }

    if ((gameTunnelObject.gameTunnelState == gameTunnelStateRunning) &&
        (currentTime - gameTunnelObject.refreshTimer) >= TUNNEL_GAME_REFRESH) {

        uint8_t shiftX = (TEXTURE_WIDTH * gameTunnelObject.animationStep) % SCREEN_WIDTH;
        uint8_t shiftY = ((TEXTURE_HEIGHT * gameTunnelObject.animationStep) >> 2) % SCREEN_HEIGHT;

        gameTunnelObject.refreshTimer = currentTime;

        gameTunnelObject.animationStep++;

        display->clear();

        /*
        for (uint8_t y = 0; y < TEXTURE_HEIGHT; y++) {
            for (uint8_t x = 0; x < TEXTURE_WIDTH; x++) {
                if (textureMap[x][y])
                    display->setPixel(x, y);
            }
        }
        */

        /*
        display->setPixel(shiftX, shiftY);
        */

        for(uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
            for(uint8_t x = 0; x < SCREEN_WIDTH; x++)
            {
                uint8_t textureY = distanceTable[y][x];
                uint8_t textureX = angleTable[y][x];

                //bool pixel = textureMap[y % TEXTURE_HEIGHT][x % TEXTURE_WIDTH];
                //bool pixel = textureMap[(uint8_t)(distanceTable[y][x] + shiftX) % TEXTURE_WIDTH][(uint8_t)(angleTable[y][x] + shiftY) % TEXTURE_HEIGHT];
                bool pixel = textureMap[(textureY + shiftX) % TEXTURE_HEIGHT][(textureX + shiftY) % TEXTURE_WIDTH];

                if (pixel)
                    display->setPixel(x, y);
            }
        }

        display->display();
    }

    *gameRemainingBudget = TUNNEL_GAME_REFRESH - (millis() - currentTime);

    if (exit->clicks > 0) {
        gameTunnelObject.gameTunnelState = gameTunnelStateIdle;
        return (gameTunnelStateExit);
    }

    return (gameTunnelObject.gameTunnelState);
}
