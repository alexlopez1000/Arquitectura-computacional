/**
 * @file Actuators.h
 * @brief Salidas fisicas: LED verde/rojo, buzzer, ventilacion e
 *        iluminacion.
 *
 * No hay cerradura mecanica ni servo [CONFIRMADO]: el acceso
 * permitido se senaliza con LED verde + buzzer y el bloqueo con LED
 * rojo + buzzer intermitente. Los patrones temporales del buzzer se
 * generan sin bloquear, con millis(), desde la tarea de actuadores.
 */

#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>

/** Senal de acceso activa. */
enum class AccessSignal : uint8_t {
    NONE = 0, ///< LEDs y buzzer apagados.
    GRANTED,  ///< LED verde fijo + tono de confirmacion.
    DENIED,   ///< Tono corto de error.
    ALARM     ///< LED rojo fijo + tono intermitente.
};

/**
 * @class Actuators
 * @brief Traduce ordenes logicas a senales electricas.
 *
 * No decide cuando actuar: recibe ordenes de AccessFSM (senal de
 * acceso) y de ComfortController (ventilacion/iluminacion).
 */
class Actuators {
public:
    /**
     * @brief Constructor.
     */
    Actuators();

    /**
     * @brief Configura pines de salida en estado seguro (todo apagado).
     */
    void begin();

    /**
     * @brief Activa una senal de acceso (reinicia su temporizacion).
     * @param signal Senal a mostrar.
     */
    void setAccessSignal(AccessSignal signal);

    /**
     * @brief Enciende/apaga la ventilacion.
     * @param on Estado deseado.
     */
    void setFan(bool on);

    /**
     * @brief Enciende/apaga la iluminacion.
     * @param on Estado deseado.
     */
    void setLight(bool on);

    /**
     * @brief Estado actual de la ventilacion.
     * @return true si esta encendida.
     */
    bool fanOn() const;

    /**
     * @brief Estado actual de la iluminacion.
     * @return true si esta encendida.
     */
    bool lightOn() const;

    /**
     * @brief Genera los patrones temporales del buzzer.
     *        Llamar periodicamente desde la tarea de actuadores.
     */
    void update();

private:
    /**
     * @brief Enciende o apaga el buzzer evitando llamadas redundantes.
     * @param on Estado deseado.
     * @param freqHz Frecuencia del tono.
     */
    void buzzer(bool on, unsigned int freqHz);

    AccessSignal signal_;     ///< Senal de acceso activa.
    uint32_t signalStartMs_;  ///< Inicio de la senal (millis()).
    bool fanOn_;              ///< Estado de la ventilacion.
    bool lightOn_;            ///< Estado de la iluminacion.
    bool buzzerOn_;           ///< Estado del buzzer.
};

#endif /* ACTUATORS_H */
