/**
 * @file Actuators.cpp
 * @brief Implementacion de los actuadores fisicos.
 */

#include "Actuators.h"
#include "config.h"

Actuators::Actuators(LockType lockType) : lockType_(lockType) {}

void Actuators::begin() {
    // TODO: pinMode salidas; estado inicial seguro (todo apagado).
    allOff();
}

void Actuators::unlock() {
    // TODO [PENDIENTE D-05]: pulso/estado segun lockType_.
}

void Actuators::lock() {
    // TODO: volver a reposo.
}

void Actuators::toggleBolt() {
    // TODO [RF-10]: accion de pestillo si aplica.
}

void Actuators::setFan(bool on)    { /* TODO: digitalWrite(PIN_FAN) */ }
void Actuators::setLight(bool on)  { /* TODO: digitalWrite(PIN_LIGHT_OUT) */ }
void Actuators::setBuzzer(bool on) { /* TODO: tono/ PWM buzzer */ }
void Actuators::setAlarmLed(bool on){ /* TODO */ }
void Actuators::setOkLed(bool on)  { /* TODO */ }

void Actuators::allOff() {
    // TODO: apagar fan, light, buzzer, LEDs; lock().
}
