/**
 * @file ComfortController.h
 * @brief Evaluacion de confort termico y control de actuadores.
 *
 * Requisito [CONFIRMADO: RF-16/17] "confort apropiado segun numero
 * de personas" con respuesta en tiempo real.
 *
 * TODO [PENDIENTE D-02]: formula/criterio exacto de confort
 * (indices PMV/SET simplificados vs umbrales T°/HR/aforo) pendiente
 * del profesor. Se implementa como funcion configurable.
 * TODO [PENDIENTE D-03]: metodo de conteo de personas.
 */

#ifndef COMFORT_CONTROLLER_H
#define COMFORT_CONTROLLER_H

#include <Arduino.h>
#include "EnvSensors.h"

/** Accion de confort calculada para los actuadores. */
struct ComfortAction {
    bool fanOn;    ///< Ventilacion forzada.
    bool lightOn;  ///< Iluminacion de apoyo.
    bool alarm;    ///< Condicion termica critica.
};

/**
 * @class ComfortController
 * @brief Decide acciones de confort a partir del snapshot ambiental.
 *
 * Modulo de alta cohesion: solo traduce variables -> accion.
 * La ejecucion fisica corresponde a Actuators (bajo acoplamiento).
 */
class ComfortController {
public:
    /**
     * @brief Constructor.
     */
    ComfortController();

    /**
     * @brief Inicializa el controlador.
     */
    void begin();

    /**
     * @brief Evalua el snapshot y retorna la accion de confort.
     * @param snap Variables ambientales actuales.
     * @return Accion calculada para los actuadores.
     */
    ComfortAction evaluate(const EnvSnapshot& snap);

    /**
     * @brief Actualizacion periodica desde task_comfort.
     */
    void update();

private:
    // TODO [PENDIENTE D-02]: funcion de decision de confort termico.
    // TODO [PENDIENTE D-03]: ponderacion por aforo (peopleCount).
};

#endif /* COMFORT_CONTROLLER_H */
