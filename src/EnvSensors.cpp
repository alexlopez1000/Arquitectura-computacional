/**
 * @file EnvSensors.cpp
 * @brief Implementacion de la adquisicion ambiental.
 */

#include "EnvSensors.h"

EnvSensors::EnvSensors()
    : dht_(PIN_DHT, DHT_TYPE),
      lightAvg_(ANALOG_AVG_SAMPLES),
      auxAvg_(ANALOG_AVG_SAMPLES),
      current_{NAN, NAN, false, 0, 0, 0} {}

void EnvSensors::begin() {
    dht_.begin();
    pinMode(PIN_LDR, INPUT);
    pinMode(PIN_ANALOG_AUX, INPUT);
}

void EnvSensors::update() {
    const float h = dht_.readHumidity();
    const float t = dht_.readTemperature();
    current_.dhtValid = !(isnan(h) || isnan(t));
    if (current_.dhtValid) {
        current_.temperatureC = t;
        current_.humidityPct = h;
    }
    current_.lightLevel = static_cast<uint16_t>(
        lightAvg_.rolling(static_cast<uint16_t>(analogRead(PIN_LDR))) + 0.5f);
    current_.auxLevel = static_cast<uint16_t>(
        auxAvg_.rolling(static_cast<uint16_t>(analogRead(PIN_ANALOG_AUX))) + 0.5f);
    current_.timestampMs = millis();
}

EnvSnapshot EnvSensors::snapshot() const { return current_; }
