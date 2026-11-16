/**
 * @file main.cpp
 * @brief Punto de entrada del sistema de control de acceso.
 *
 * Inicializa los modulos y registra las tareas asincronas en el
 * TaskScheduler. loop() solo despacha tareas (super-loop que
 * delega en scheduler cooperativo).
 *
 * TODO [PENDIENTE D-04]: si se confirma FreeRTOS, main.cpp se
 * reescribe con xTaskCreate; los modulos no cambian.
 */

#include <Arduino.h>
#include "config.h"
#include "TaskScheduler.h"
// TODO: includes de modulos al implementar:
// #include "AccessFSM.h"
// #include "AuthManager.h"
// #include "KeypadReader.h"
// #include "RfidReader.h"
// #include "UserManager.h"
// #include "NvmStorage.h"
// #include "ScheduleValidator.h"
// #include "EnvSensors.h"
// #include "ComfortController.h"
// #include "Actuators.h"
// #include "DisplayUI.h"
// #include "EventLog.h"

/** Scheduler global con capacidad para las 8 tareas del diseno. */
static TaskScheduler scheduler(8);

/**
 * @brief Inicializacion del sistema (una sola vez al arrancar).
 *
 * Configura Serial y, en implementaciones futuras, instancia los
 * modulos con los pines de config.h y registra las tareas periodicas
 * en el scheduler global.
 */
void setup() {
    Serial.begin(9600);
    // TODO [PENDIENTE D-05]: inicializar modulos con pines de config.h.

    // TODO: registrar tareas periodicas:
    // scheduler.addTask("fsm",       taskFsm,       TASK_PERIOD_FSM_MS);
    // scheduler.addTask("keypad",    taskKeypad,    TASK_PERIOD_KEYPAD_MS);
    // scheduler.addTask("rfid",      taskRfid,      TASK_PERIOD_RFID_MS);
    // scheduler.addTask("sensors",   taskSensors,   TASK_PERIOD_SENSORS_MS);
    // scheduler.addTask("comfort",   taskComfort,   TASK_PERIOD_COMFORT_MS);
    // scheduler.addTask("ui",        taskUi,        TASK_PERIOD_UI_MS);
    // scheduler.addTask("storage",   taskStorage,   TASK_PERIOD_STORAGE_MS);
    // scheduler.addTask("schedule",  taskSchedule,  TASK_PERIOD_SCHEDULE_MS);
}

/**
 * @brief Bucle principal: despacha las tareas asincronas.
 *
 * Llamado repetidamente por el nucleo de Arduino; delega en
 * TaskScheduler::update() sin bloquear el procesador.
 */
void loop() {
    scheduler.update(); // Despacho no bloqueante de tareas.
}
