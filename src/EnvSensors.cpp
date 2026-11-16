/**
 * @file EnvSensors.cpp
 * @brief Implementacion de la adquisicion ambiental.
 */

#include "EnvSensors.h"
#include <DHT.h>
#include <Average.h>

EnvSensors::EnvSensors() {}

void EnvSensors::begin() {
    // TODO: DHT.begin(); pinMode sensores digitales/analogicos.
}

void EnvSensors::update() {
    // TODO: leer DHT11 (lento, ~2s), HC-SR04 (pulseIn),
    // sensor fuego y luz; aplicar filtro Average; actualizar snapshot.
}

EnvSnapshot EnvSensors::snapshot() const { return current_; }
