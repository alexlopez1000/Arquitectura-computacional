/**
 * @file EnvSensors.h
 * @brief Adquisicion de variables ambientales: DHT11 (temperatura y
 *        humedad), LDR (A0) y sensor analogico adicional (A1).
 *
 * Las lecturas analogicas se suavizan con promedio movil (libreria
 * Average del curso). El DHT11 se lee como maximo cada 2 s.
 */

#ifndef ENV_SENSORS_H
#define ENV_SENSORS_H

#include <Arduino.h>
#include <DHT.h>
#include <Average.h>
#include "config.h"

/** Instantanea de variables ambientales. */
struct EnvSnapshot {
    float    temperatureC; ///< Temperatura del aire (C).
    float    humidityPct;  ///< Humedad relativa (%).
    bool     dhtValid;     ///< true si la ultima lectura DHT11 fue valida.
    uint16_t lightLevel;   ///< Nivel de luz LDR promediado (ADC 0-1023).
    uint16_t auxLevel;     ///< Sensor analogico adicional promediado (ADC 0-1023).
    uint32_t timestampMs;  ///< Instante de adquisicion (millis()).
};

/**
 * @class EnvSensors
 * @brief Muestrea y filtra los sensores ambientales.
 *
 * Publica snapshots que consumen ComfortController y DisplayUI.
 */
class EnvSensors {
public:
    /**
     * @brief Constructor.
     */
    EnvSensors();

    /**
     * @brief Inicializa el DHT11 y las entradas analogicas.
     */
    void begin();

    /**
     * @brief Ciclo de muestreo. Llamar desde la tarea ambiental.
     */
    void update();

    /**
     * @brief Snapshot mas reciente.
     * @return Copia del ultimo snapshot.
     */
    EnvSnapshot snapshot() const;

private:
    DHT dht_;                     ///< Driver DHT11.
    Average<uint16_t> lightAvg_;  ///< Promedio movil del LDR.
    Average<uint16_t> auxAvg_;    ///< Promedio movil del sensor adicional.
    EnvSnapshot current_;         ///< Ultimo snapshot publicado.
};

#endif /* ENV_SENSORS_H */
