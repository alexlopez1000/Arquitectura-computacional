/**
 * @file DoorSensor.h
 * @brief Estado de la puerta mediante sensor de efecto Hall.
 *
 * El iman en la hoja de la puerta activa el sensor (salida LOW)
 * cuando la puerta esta cerrada. Se aplica anti-rebote por tiempo.
 */

#ifndef DOOR_SENSOR_H
#define DOOR_SENSOR_H

#include <Arduino.h>

/**
 * @class DoorSensor
 * @brief Lectura con anti-rebote del sensor Hall de la puerta.
 */
class DoorSensor {
public:
    /**
     * @brief Constructor.
     * @param pin Pin digital del sensor Hall.
     * @param debounceMs Tiempo de estabilidad requerido (ms).
     */
    DoorSensor(uint8_t pin, uint16_t debounceMs);

    /**
     * @brief Configura el pin como entrada con pull-up.
     */
    void begin();

    /**
     * @brief Muestrea el sensor. Llamar desde la tarea del sensor Hall.
     */
    void update();

    /**
     * @brief Estado filtrado de la puerta.
     * @return true si la puerta esta abierta.
     */
    bool isOpen() const;

private:
    uint8_t  pin_;          ///< Pin del sensor.
    uint16_t debounceMs_;   ///< Ventana de anti-rebote.
    bool     stableOpen_;   ///< Estado filtrado.
    bool     lastRawOpen_;  ///< Ultima lectura cruda.
    uint32_t lastChangeMs_; ///< Instante del ultimo cambio crudo.
};

#endif /* DOOR_SENSOR_H */
