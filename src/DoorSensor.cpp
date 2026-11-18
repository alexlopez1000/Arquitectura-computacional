/**
 * @file DoorSensor.cpp
 * @brief Implementacion del sensor Hall de puerta.
 */

#include "DoorSensor.h"

DoorSensor::DoorSensor(uint8_t pin, uint16_t debounceMs)
    : pin_(pin), debounceMs_(debounceMs), stableOpen_(false),
      lastRawOpen_(false), lastChangeMs_(0) {}

void DoorSensor::begin() {
    pinMode(pin_, INPUT_PULLUP);
    lastRawOpen_ = stableOpen_ = (digitalRead(pin_) == HIGH);
    lastChangeMs_ = millis();
}

void DoorSensor::update() {
    const bool rawOpen = (digitalRead(pin_) == HIGH);
    const uint32_t now = millis();
    if (rawOpen != lastRawOpen_) {
        lastRawOpen_ = rawOpen;
        lastChangeMs_ = now;
    } else if (rawOpen != stableOpen_ && (now - lastChangeMs_) >= debounceMs_) {
        stableOpen_ = rawOpen;
    }
}

bool DoorSensor::isOpen() const { return stableOpen_; }
