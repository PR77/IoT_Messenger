#include "batteryHistogram.h"

//=============================================================================
// Object constructors
//=============================================================================

BatteryHistogram::BatteryHistogram(bool filterVoltage) {
    for (uint8_t i = 0; i < BATTERY_VOLTAGE_SAMPLES_MAX; i++) {
        this->_batteryVoltageSamples[i] = 0;
    }

    _currentSample = 0;
    _refreshTimer = 0;
    _sampleTimer = 0;
    _filterVoltage = filterVoltage;

    _lpfBeta = 0.025;
    _filteredVoltage = 511.0;
}

//=============================================================================
// Public functions
//=============================================================================

void BatteryHistogram::Init(void) {
    for (uint8_t i = 0; i < BATTERY_VOLTAGE_SAMPLES_MAX; i++) {
        this->_batteryVoltageSamples[i] = 0;
    }

    _currentSample = 0;
    _refreshTimer = 0;
    _sampleTimer = 0;
}

void BatteryHistogram::Update() {

    uint32_t currentTime = millis();
    uint16_t currentVoltage;

    if ((currentTime - _sampleTimer) >= ADC_SAMPLE_INTERVAL_DELAY_M_SECONDS) {
        // https://github.com/esp8266/Arduino/issues/1634

        _sampleTimer = currentTime;

        _lastSampleValue = analogRead(A0);
    }

    currentVoltage = _lastSampleValue;

    if (_filterVoltage == true) {
        // LPF: Y(n) = (1-ß)*Y(n-1) + (ß*X(n))) = Y(n-1) - (ß*(Y(n-1)-X(n)));
        _filteredVoltage = _filteredVoltage - (_lpfBeta * (_filteredVoltage - (float)currentVoltage));
        currentVoltage = (uint16_t)_filteredVoltage;
    }

    if ((currentTime - _refreshTimer) >= BATTERY_VOLTAGE_SAMPLE_INTERNAL_M_SECONDS) {

        _refreshTimer = currentTime;

        // stuff a new sample into the sample array
        if (_currentSample < BATTERY_VOLTAGE_SAMPLES_MAX) {
            this->_batteryVoltageSamples[_currentSample] = currentVoltage;
        }

        // sample array full, so now make it a sliding window array
        if (_currentSample == BATTERY_VOLTAGE_SAMPLES_MAX) {
            for (uint8_t i = 0; i < (BATTERY_VOLTAGE_SAMPLES_MAX - 1); i++) {
                this->_batteryVoltageSamples[i] = this->_batteryVoltageSamples[i + 1];
            }

            this->_batteryVoltageSamples[BATTERY_VOLTAGE_SAMPLES_MAX - 1] = currentVoltage;
        } else {
            _currentSample++;
        }
    }
}

uint16_t * BatteryHistogram::GetBatteryHistogram(void) {
    return (this->_batteryVoltageSamples);
}
