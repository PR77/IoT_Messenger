#include "beeperControl.h"

//=============================================================================
// Object constructors
//=============================================================================

BeeperControl::BeeperControl(uint8_t beeperPin) {
    _pin = beeperPin;
    _numberOfBeeps = 0;
    _processedBeepCount = 0;
    _lastUpdateTime = 0;
    _state = beepHandlerIdle;

    beeperFrequency = 1000;
    beeperDuration = 75;
    deleyBetweenBeeps = 30;
}

BeeperControl::BeeperControl(uint8_t beeperPin, uint16_t beeperFrequency) {
    _pin = beeperPin;
    _numberOfBeeps = 0;
    _processedBeepCount = 0;
    _lastUpdateTime = 0;
    _state = beepHandlerIdle;

    beeperFrequency = beeperFrequency;
    beeperDuration = 75;
    deleyBetweenBeeps = 30;
}

BeeperControl::BeeperControl(uint8_t beeperPin, uint16_t beeperFrequency, uint16_t beeperDuration) {
    _pin = beeperPin;
    _numberOfBeeps = 0;
    _processedBeepCount = 0;
    _lastUpdateTime = 0;
    _state = beepHandlerIdle;

    beeperFrequency = beeperFrequency;
    beeperDuration = beeperDuration;
    deleyBetweenBeeps = 30;
}

BeeperControl::BeeperControl(uint8_t beeperPin, uint16_t beeperFrequency, uint16_t beeperDuration, uint16_t deleyBetweenBeeps) {
    _pin = beeperPin;
    _numberOfBeeps = 0;
    _processedBeepCount = 0;
    _lastUpdateTime = 0;
    _state = beepHandlerIdle;

    beeperFrequency = beeperFrequency;
    beeperDuration = beeperDuration;
    deleyBetweenBeeps = deleyBetweenBeeps;
}

//=============================================================================
// Public functions
//=============================================================================

void BeeperControl::Update() {

    uint32_t currentTime = millis();

    switch (_state) {
        case beepHandlerIdle:
        {
            if (_numberOfBeeps > _processedBeepCount)
                _state = beepHandlerSoundPhase;
        }
        break;

        case beepHandlerSoundPhase:
        {
            _lastUpdateTime = currentTime;
            tone(_pin, beeperFrequency, beeperDuration);
            _state = beepHandlerPausePhase;
        }
        break;

        case beepHandlerPausePhase:
        {
            if ((currentTime - _lastUpdateTime) >=  beeperDuration + deleyBetweenBeeps) {
                _state = beepHandlerRepeatCheck;
            }
        }
        break;

        case beepHandlerRepeatCheck:
        {
            _processedBeepCount++;

            if (_numberOfBeeps != _processedBeepCount) {
                _state = beepHandlerSoundPhase;
            }
            else {
                _processedBeepCount = 0;
                _numberOfBeeps = 0;
                noTone(_pin);
                _state = beepHandlerIdle;
            }
        }
        break;

        case beepHandlerBlockBeeper:
            break;

        case beepHandlerUnblockBeeper:
        default:
            _state = beepHandlerIdle;
            _processedBeepCount = 0;
            noTone(_pin);
            break;
    }
}

void BeeperControl::RequestBeeper(uint8_t numberOfBeeps) {
    _numberOfBeeps = numberOfBeeps;
}

void BeeperControl::RequestBlock() {
    _state = beepHandlerBlockBeeper;
}

void BeeperControl::RequestUnblock() {
    _state = beepHandlerUnblockBeeper;
}

beepHandler_t BeeperControl::GetBeeperState() {
    return (_state);
}
