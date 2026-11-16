/**
 * @file EnvSensors.h
 * @brief Adquisicion de variables ambientales y aforo.
 *
 * Requisitos [CONFIRMADO: RF-15..17]: temperatura, humedad, fuego,
 * iluminacion y conteo de personas para el calculo de confort.
 *
 * Librerias del curso: DHT (DHT11) [Lab_DIG_TTM_02], HC-SR04 para
 * distancia/aforo, sensor de llama y LDR pendientes de confirmar.
 */

#ifndef ENV_SENSORS_H
#define ENV_SENSORS_H

#include <Arduino.h>
#include "config.h"

/** Instantanea de variables ambientales para consumo por tareas. */
struct EnvSnapshot {
    float temperatureC;   ///< Temperatura en grados Celsius.
    float humidityPct;    ///< Humedad relativa porcentaje.
    bool  fireDetected;   ///< Lectura digital de fuego.
    uint16_t lightLevel;  ///< Nivel de iluminacion (0-1023 ADC).
    uint8_t peopleCount;  ///< Personas estimadas en el area.
    unsigned long timestampMs; ///< Marca de adquisicion.
};

/**
 * @class EnvSensors
 * @brief Lee y promedia los sensores ambientales.
 *
 * Responsabilidad unica: muestreo periodico y filtrado (libreria
 * Average del curso) de sensores. Publica snapshots inmutables que
 * consumen ComfortController y DisplayUI (bajo acoplamiento).
 */
class EnvSensors {
public:
    /**
     * @brief Constructor.
     */
    EnvSensors();

    /**
     * @brief Configura pines y librerias de sensores.
     */
    void begin();

    /**
     * @brief Ciclo de muestreo. Llamar desde task_sensors_env.
     *        El DHT11 es lento (~2 s): la lectura se espacia.
     */
    void update();

    /**
     * @brief Snapshot mas reciente de variables ambientales.
     * @return Copia del ultimo snapshot valido.
     */
    EnvSnapshot snapshot() const;

private:
    EnvSnapshot current_; ///< Ultimo snapshot publicado.
    // TODO [PENDIENTE D-02/D-03/D-05]: objetos DHT, HC-SR04,
    // filtro Average por canal y algoritmo de conteo de personas.
};

#endif /* ENV_SENSORS_H */
