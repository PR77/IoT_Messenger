#ifndef BEEPER_H
#define BEEPER_H

#include "Arduino.h"

//=============================================================================
// Types
//=============================================================================

typedef enum {

    beepHandlerIdle = 0,
    beepHandlerSoundPhase,
    beepHandlerPausePhase,
    beepHandlerRepeatCheck
} beepHandler_t;

//=============================================================================
// Classes
//=============================================================================

class Beeper
{
    public:
        Beeper(uint8_t beeperPin);
        Beeper(uint8_t beeperPin, uint16_t beeperFrequency);
        Beeper(uint8_t beeperPin, uint16_t beeperFrequency, uint16_t beeperDuration);
        Beeper(uint8_t beeperPin, uint16_t beeperFrequency, uint16_t beeperDuration, uint16_t deleyBetweenBeeps);

        void Update();
        void RequestBeeper(uint8_t numberOfBeeps);
        beepHandler_t GetBeeperState();

        uint16_t beeperFrequency;
        uint16_t beeperDuration;
        uint16_t deleyBetweenBeeps;

    private:
        uint8_t _pin;
        uint8_t _numberOfBeeps;
        uint16_t _processedBeepCount;
        uint32_t _lastUpdateTime;
        beepHandler_t _state;
};

#endif // BEEPER_H
