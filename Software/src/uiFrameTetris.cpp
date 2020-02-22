#include "uiGlobal.h"
#include "uiFrameTetris.h"

#include <ESP8266WiFi.h>

const uint8_t tetrisLogo[] PROGMEM = {

    // https://www.dcode.fr/binary-image - LSB needs to be first in image data

    B11111111, B11000111, B01111111, B11111100, B10011111, B11111111, B11000001, B11111001, B10011111,
    B11111111, B11000111, B00111111, B11111100, B10011111, B10000011, B11000000, B11111001, B10001000,
    B11111110, B11000111, B00111111, B11111100, B10011111, B11000011, B11000000, B01111001, B00000000,
    B11110000, B11000000, B00000001, B11000000, B10000001, B01000011, B00000000, B01110000, B00000000,
    B11110000, B11000000, B00000001, B11000000, B10000001, B01100011, B00000000, B11110000, B00000000,
    B11110000, B11000000, B00000001, B11000000, B10000001, B00110011, B11000000, B11100001, B00000000,
    B11110000, B11000000, B00011111, B11000000, B10000001, B00110011, B11000000, B11100001, B00000001,
    B11110000, B11000000, B00001111, B11000000, B10000001, B11100011, B11000001, B11000001, B00000001,
    B11110000, B11000000, B00000001, B11000000, B10000001, B11100011, B11000011, B11000001, B00000011,
    B11110000, B11000000, B00000001, B11000000, B10000001, B11000011, B11000011, B10000001, B00000011,
    B11110000, B11000000, B00000001, B11000000, B10000001, B11000011, B11000111, B10000001, B00000111,
    B11110000, B11000000, B00000001, B11000000, B10000001, B10000011, B11000111, B00000001, B00000111,
    B11110000, B11000000, B00000001, B11000000, B10000001, B10000011, B11001111, B00000001, B00001111,
    B11110000, B11000000, B00000001, B11000000, B10000001, B00000011, B11001111, B00000001, B00001110,
    B11110000, B11000000, B00000001, B11000000, B10000001, B00000011, B10011110, B00000001, B00011110,
    B11110000, B11000000, B00000001, B11000000, B10000001, B00000011, B00011110, B00000001, B00011100,
    B11110000, B11000000, B00000001, B11000000, B10000001, B00000011, B00111100, B00000001, B00111100,
    B11110000, B11000000, B11111111, B11000001, B10000001, B00000011, B00111100, B11111000, B00111111,
    B11110000, B11000000, B11111111, B11000011, B10000001, B00000011, B01101000, B11111000, B00111111,
    B11110000, B11000000, B11111111, B11000011, B10000001, B00000011, B01111000, B11111000, B00111111

};

void uiFrameTetris(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
    display->drawXbm(28 + x, 12 + y, 72, 20, tetrisLogo);
}