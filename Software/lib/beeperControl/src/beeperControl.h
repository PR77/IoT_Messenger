#ifndef BEEPER_CONTROL_H
#define BEEPER_CONTROL_H

#include "Arduino.h"

//=============================================================================
// Types
//=============================================================================

typedef enum {

    beepHandlerIdle = 0,
    beepHandlerSoundPhase,
    beepHandlerPausePhase,
    beepHandlerRepeatCheck,
    beepHandlerBlockBeeper,
    beepHandlerUnblockBeeper
} beepHandler_t;

//=============================================================================
// Classes
//=============================================================================

class BeeperControl
{
    public:
        BeeperControl(uint8_t beeperPin);
        BeeperControl(uint8_t beeperPin, uint16_t beeperFrequency);
        BeeperControl(uint8_t beeperPin, uint16_t beeperFrequency, uint16_t beeperDuration);
        BeeperControl(uint8_t beeperPin, uint16_t beeperFrequency, uint16_t beeperDuration, uint16_t deleyBetweenBeeps);

        void Update();
        void RequestBeeper(uint8_t numberOfBeeps);
        void RequestBlock();
        void RequestUnblock();

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

#endif // BEEPER_CONTROL_H
