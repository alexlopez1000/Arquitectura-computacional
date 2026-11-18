/**
 * @file Actuators.cpp
 * @brief Implementacion de los actuadores fisicos.
 */

#include "Actuators.h"
#include "config.h"

namespace {
const uint8_t kRelayOn  = RELAY_ACTIVE_LEVEL;
const uint8_t kRelayOff = (RELAY_ACTIVE_LEVEL == HIGH) ? LOW : HIGH;
} // namespace

Actuators::Actuators()
    : signal_(AccessSignal::NONE), signalStartMs_(0),
      fanOn_(false), lightOn_(false), buzzerOn_(false) {}

void Actuators::begin() {
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_FAN, OUTPUT);
    pinMode(PIN_LIGHT_OUT, OUTPUT);
    setAccessSignal(AccessSignal::NONE);
    setFan(false);
    setLight(false);
}

void Actuators::setAccessSignal(AccessSignal signal) {
    signal_ = signal;
    signalStartMs_ = millis();
    digitalWrite(PIN_LED_GREEN, signal == AccessSignal::GRANTED ? HIGH : LOW);
    digitalWrite(PIN_LED_RED, (signal == AccessSignal::DENIED || signal == AccessSignal::ALARM) ? HIGH : LOW);
    buzzerOn_ = true;  // fuerza la actualizacion del tono
    buzzer(false, 0);
    update();
}

void Actuators::setFan(bool on) {
    fanOn_ = on;
    digitalWrite(PIN_FAN, on ? kRelayOn : kRelayOff);
}

void Actuators::setLight(bool on) {
    lightOn_ = on;
    digitalWrite(PIN_LIGHT_OUT, on ? kRelayOn : kRelayOff);
}

bool Actuators::fanOn() const { return fanOn_; }
bool Actuators::lightOn() const { return lightOn_; }

void Actuators::update() {
    const uint32_t elapsed = millis() - signalStartMs_;  // seguro ante overflow
    switch (signal_) {
        case AccessSignal::GRANTED:
            buzzer(elapsed < GRANTED_BEEP_MS, BUZZER_FREQ_OK_HZ);
            break;
        case AccessSignal::DENIED:
            buzzer(elapsed < ERR_BEEP_MS, BUZZER_FREQ_ERR_HZ);
            break;
        case AccessSignal::ALARM:
            buzzer((elapsed % ALARM_BEEP_PERIOD_MS) < (ALARM_BEEP_PERIOD_MS / 2),
                   BUZZER_FREQ_ERR_HZ);
            break;
        case AccessSignal::NONE:
        default:
            buzzer(false, 0);
            break;
    }
}

void Actuators::buzzer(bool on, unsigned int freqHz) {
    if (on == buzzerOn_) return;
    buzzerOn_ = on;
    if (on) {
        tone(PIN_BUZZER, freqHz);
    } else {
        noTone(PIN_BUZZER);
    }
}
