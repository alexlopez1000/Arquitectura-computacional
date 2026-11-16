/**
 * @file TaskScheduler.cpp
 * @brief Implementacion del scheduler cooperativo de tareas.
 */

#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(uint8_t maxTasks)
    : tasks_(nullptr), count_(0), max_(maxTasks) {
    // TODO: asignar arreglo de AsyncTask (new/malloc).
}

bool TaskScheduler::addTask(const char* name, TaskFunction fn, unsigned long period_ms) {
    // TODO [PENDIENTE D-04]: registrar tarea; validar cupo.
    return false;
}

bool TaskScheduler::setEnabled(const char* name, bool enabled) {
    // TODO: buscar por nombre y conmutar flag enabled.
    return false;
}

void TaskScheduler::update() {
    // TODO: recorrer tareas y ejecutar fn() cuando venza el periodo.
}
