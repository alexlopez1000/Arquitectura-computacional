/**
 * @file TaskScheduler.h
 * @brief Scheduler cooperativo de tareas asincronas.
 *
 * Abstrae el mecanismo de concurrencia del sistema. La clase de RTOS
 * (ARQC_TASK.pdf) exige alta cohesion y bajo acoplamiento: cada tarea
 * tiene una unica responsabilidad y se comunica por colas/eventos,
 * nunca por variables globales compartidas.
 *
 * TODO [PENDIENTE D-04]: confirmar con el profesor si se usa
 * programacion cooperativa (millis()/AsyncTask-style, implementada
 * aqui) o FreeRTOS. Si se elige FreeRTOS, esta clase se reemplaza por
 * xTaskCreate + colas, manteniendo la misma API de modulos.
 */

#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#include <Arduino.h>
#include "config.h"

/** Firma comun de todas las tareas del sistema. */
typedef void (*TaskFunction)();

/**
 * @brief Tarea periodica registrada en el scheduler.
 *
 * Cada tarea se ejecuta cuando transcurre su periodo; el scheduler
 * garantiza que ninguna tarea bloquee a las demas (coopera cediendo
 * el control en cada ciclo de loop()).
 */
struct AsyncTask {
    const char*   name;        ///< Nombre para depuracion/Doxygen.
    TaskFunction  function;    ///< Funcion a invocar periodicamente.
    unsigned long period_ms;   ///< Periodo de ejecucion en ms.
    unsigned long last_run_ms; ///< Marca de la ultima ejecucion.
    bool          enabled;     ///< Habilitada/deshabilitada en runtime.
};

/**
 * @class TaskScheduler
 * @brief Registro y despacho no bloqueante de tareas asincronas.
 *
 * @note Diseno cooperativo: las funciones de tarea deben ser cortas
 *       y no usar delay(); si una tarea tarda mas que su periodo se
 *       pierde el tick (comportamiento documentado para la version
 *       cooperativa).
 */
class TaskScheduler {
public:
    /**
     * @brief Constructor.
     * @param maxTasks Numero maximo de tareas registrables.
     */
    explicit TaskScheduler(uint8_t maxTasks);

    /**
     * @brief Registra una tarea periodica.
     * @param name Nombre identificador de la tarea.
     * @param fn Funcion de la tarea.
     * @param period_ms Periodo en milisegundos.
     * @return true si se registro; false si no hay cupo.
     */
    bool addTask(const char* name, TaskFunction fn, unsigned long period_ms);

    /**
     * @brief Habilita o deshabilita una tarea por nombre.
     * @param name Nombre de la tarea.
     * @param enabled Estado deseado.
     * @return true si la tarea existe.
     */
    bool setEnabled(const char* name, bool enabled);

    /**
     * @brief Despacha las tareas cuyo periodo vencio.
     *        Debe llamarse en cada iteracion de loop().
     */
    void update();

private:
    AsyncTask* tasks_;   ///< Arreglo dinamico de tareas.
    uint8_t    count_;   ///< Tareas registradas.
    uint8_t    max_;     ///< Capacidad maxima.
};

#endif /* TASK_SCHEDULER_H */
