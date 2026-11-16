/**
 * @file Actuators.h
 * @brief Accionamiento de cerradura, ventilacion, iluminacion,
 *        LEDs y buzzer.
 *
 * Cubre los actuadores del enunciado: cerradura con desbloqueo
 * temporal [RF-03], ventilacion/iluminacion de confort [RF-17],
 * alarmas visuales y sonoras [RF-04, RF-21] y pestillo [RF-10].
 *
 * TODO [PENDIENTE D-05]: tipo real de cerradura (solenoide, relé,
 * servo o combinacion pestillo) — la interfaz se mantiene neutra.
 */

#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>

/** Tipo de actuador de cerradura [PENDIENTE D-05]. */
enum class LockType : uint8_t {
    SOLENOID = 0,  ///< Solenoide electrico.
    SERVO,         ///< Servomotor de accion.
    RELAY_COMBO    ///< Relé + pestillo combinados [RF-10].
};

/**
 * @class Actuators
 * @brief Escribe los estados fisicos de todos los actuadores.
 *
 * Responsabilidad unica: translation estado logico -> senal
 * electrica. No decide CUANDO actuar; recibe ordenes de la FSM y
 * del ComfortController.
 */
class Actuators {
public:
    /**
     * @brief Constructor.
     * @param lockType Tipo de cerradura instalada.
     */
    explicit Actuators(LockType lockType = LockType::SOLENOID);

    /**
     * @brief Configura pines de salida y estado seguro inicial.
     */
    void begin();

    /* ---- Cerradura ---- */

    /**
     * @brief Desbloquea la cerradura.
     */
    void unlock();

    /**
     * @brief Bloquea la cerradura (reposo).
     */
    void lock();

    /**
     * @brief Acciona el pestillo si el mecanismo lo incluye [RF-10].
     */
    void toggleBolt();

    /* ---- Confort ---- */

    /**
     * @brief Enciende/apaga la ventilacion.
     * @param on Estado deseado.
     */
    void setFan(bool on);

    /**
     * @brief Enciende/apaga la iluminacion de confort.
     * @param on Estado deseado.
     */
    void setLight(bool on);

    /* ---- Alarmas ---- */

    /**
     * @brief Enciende/apaga el buzzer de alarma.
     * @param on Estado deseado.
     */
    void setBuzzer(bool on);

    /**
     * @brief Enciende/apaga el LED rojo de alarma.
     * @param on Estado deseado.
     */
    void setAlarmLed(bool on);

    /**
     * @brief Enciende/apaga el LED verde de estado OK.
     * @param on Estado deseado.
     */
    void setOkLed(bool on);

    /**
     * @brief Apaga todos los actuadores (estado seguro).
     */
    void allOff();

private:
    LockType lockType_; ///< Tipo de cerradura configurado.
    // TODO [PENDIENTE D-05]: driver Servo/relé segun lockType.
};

#endif /* ACTUATORS_H */
