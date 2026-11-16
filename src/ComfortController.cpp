/**
 * @file ComfortController.cpp
 * @brief Implementacion del evaluador de confort termico.
 */

#include "ComfortController.h"
#include "config.h"

ComfortController::ComfortController() {}

void ComfortController::begin() {
    // TODO [PENDIENTE D-02/D-03]: cargar umbrales/ponderacion.
}

ComfortAction ComfortController::evaluate(const EnvSnapshot& snap) {
    ComfortAction action = { false, false, false };
    // TODO [PENDIENTE D-02]: decision de confort termico segun
    // temperatura, humedad y aforo (snap.peopleCount).
    // TODO [PENDIENTE D-03]: metodo de conteo de personas.
    return action;
}

void ComfortController::update() {
    // TODO: obtener snapshot y publicar ComfortAction a Actuators.
}
