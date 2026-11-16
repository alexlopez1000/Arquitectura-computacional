/**
 * @file ScheduleValidator.cpp
 * @brief Implementacion de validacion por franjas horarias.
 */

#include "ScheduleValidator.h"
#include <Wire.h>

ScheduleValidator::ScheduleValidator() {}

void ScheduleValidator::begin() {
    // TODO [PENDIENTE D-05]: Wire.begin(); init RTC DS3231.
}

void ScheduleValidator::update() {
    // TODO: leer hora/minuto/dia del RTC.
}

bool ScheduleValidator::isAccessAllowed() const {
    // TODO: comparar hora actual contra franjas registradas.
    return false;
}

void ScheduleValidator::addWindow(const TimeWindow& window) {
    // TODO: almacenar franja en tabla configurable.
}
