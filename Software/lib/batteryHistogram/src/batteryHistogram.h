#ifndef BATTERY_HISTOGRAM_H
#define BATTERY_HISTOGRAM_H

#include "Arduino.h"

//=============================================================================
// Defines
//=============================================================================

#define BATTERY_VOLTAGE_SAMPLES_MAX                 128

#define BATTERY_VOLTAGE_SAMPLE_INTERNAL_MINUTES     5
#define BATTERY_VOLTAGE_SAMPLE_INTERVAL_SECONDS     ((BATTERY_VOLTAGE_SAMPLE_INTERNAL_MINUTES * 60) / BATTERY_VOLTAGE_SAMPLES_MAX)
#define BATTERY_VOLTAGE_SAMPLE_INTERNAL_M_SECONDS   BATTERY_VOLTAGE_SAMPLE_INTERVAL_SECONDS * 1000L

#define ADC_SAMPLE_INTERVAL_DELAY_M_SECONDS         20

//=============================================================================
// Classes
//=============================================================================

class BatteryHistogram
{
    public:
        BatteryHistogram(bool filterVoltage = false);

        void Init(void);
        void Update();
        uint16_t *GetBatteryHistogram(void);

    private:
        uint16_t _batteryVoltageSamples[BATTERY_VOLTAGE_SAMPLES_MAX];
        uint16_t _lastSampleValue;
        uint8_t _currentSample;
        uint32_t _refreshTimer;
        uint32_t _sampleTimer;
        bool _filterVoltage;

        float _lpfBeta;
        float _filteredVoltage;
};

#endif // BATTERY_HISTOGRAM_H
