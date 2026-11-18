/**
 * @file ComfortController.h
 * @brief Confort termico por indice PMV (Fanger, ISO 7730) y
 *        control automatico de ventilacion e iluminacion.
 *
 * Entradas: temperatura del aire Ta y humedad relativa HR del DHT11.
 * Parametros estandar de interiores: M = 1.2 met, Icl = 0.5 clo,
 * v = 0.1 m/s, W = 0 y temperatura radiante media Tr = Ta.
 *
 * Ley de control de la ventilacion (histeresis):
 *  - PMV > +0.5  -> ventilacion ON.
 *  - PMV <= +0.2 -> ventilacion OFF.
 *  - En medio se conserva el estado anterior.
 */

#ifndef COMFORT_CONTROLLER_H
#define COMFORT_CONTROLLER_H

#include <Arduino.h>
#include "EnvSensors.h"

/** Resultado de la evaluacion de confort. */
struct ComfortAction {
    bool  valid;   ///< true si hay datos validos del DHT11.
    float pmv;     ///< Predicted Mean Vote (-3 frio .. +3 caluroso).
    float ppd;     ///< Predicted Percentage Dissatisfied (%).
    bool  fanOn;   ///< Orden para la ventilacion.
    bool  lightOn; ///< Orden para la iluminacion.
};

/**
 * @class ComfortController
 * @brief Calcula PMV/PPD y decide las salidas de confort.
 *
 * Alta cohesion: solo traduce variables -> accion. La escritura de
 * pines corresponde a Actuators.
 */
class ComfortController {
public:
    /**
     * @brief Constructor.
     */
    ComfortController();

    /**
     * @brief Evalua un snapshot ambiental y actualiza las ordenes.
     * @param snap Variables ambientales actuales.
     * @return Accion calculada (conserva el estado previo si el DHT11 fallo).
     */
    ComfortAction evaluate(const EnvSnapshot& snap);

    /**
     * @brief Ultima accion calculada.
     * @return Copia de la ultima accion.
     */
    ComfortAction lastAction() const;

    /**
     * @brief Indice PMV segun la ecuacion de Fanger (ISO 7730, anexo D).
     * @param ta Temperatura del aire (C).
     * @param tr Temperatura radiante media (C).
     * @param vel Velocidad relativa del aire (m/s).
     * @param rh Humedad relativa (%).
     * @param met Tasa metabolica (met).
     * @param clo Aislamiento de la ropa (clo).
     * @param wme Trabajo externo (met).
     * @return Valor PMV.
     */
    static float computePmv(float ta, float tr, float vel, float rh,
                            float met, float clo, float wme);

    /**
     * @brief Porcentaje de insatisfechos a partir del PMV.
     * @param pmv Indice PMV.
     * @return PPD en porcentaje (5..100).
     */
    static float computePpd(float pmv);

private:
    ComfortAction action_; ///< Ultima accion (memoria para histeresis).
};

#endif /* COMFORT_CONTROLLER_H */
