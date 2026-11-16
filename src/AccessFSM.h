/**
 * @file AccessFSM.h
 * @brief Maquina de estados finitos principal del sistema de acceso.
 *
 * Implementa el nucleo FSM exigido por la rubrica (8%): estados
 * modulares IDLE/ERR/OPEN/ALR visibles en LCD, con sub-estados
 * internos para autenticacion.
 *
 * Patron segun material de clase (Lab_DIG_TTM_04.pdf):
 *  - Variante "Library": StateMachine con AddTransition/Update
 *    (FSM_Library.ino) — PROBABLE ELECCION [PROPUESTA TÉCNICA].
 *  - Alternativas documentadas: FSM por funciones (FSM1.ino) y
 *    FSM tabular en PROGMEM (FSM_Tabular.ino).
 *
 * TODO [PENDIENTE D-08]: definir si el confort termico es una FSM
 * independiente o salidas/estados de esta FSM. Propuesta: modulo
 * separado ComfortController consultado por la FSM.
 */

#ifndef ACCESS_FSM_H
#define ACCESS_FSM_H

#include <Arduino.h>

/** Estados principales expuestos en LCD [CONFIRMADO: enunciado]. */
enum class FsmState : uint8_t {
    IDLE = 0,  ///< Reposo: sistema armado, esperando autenticacion.
    ERR  = 1,  ///< Error: credencial invalida o evento rechazado.
    OPEN = 2,  ///< Abierto: cerradura desbloqueada, cuenta regresiva.
    ALR  = 3   ///< Alarma: intrusion, 3 fallos o fuego detectado.
};

/** Sub-estados internos del flujo de autenticacion [PROPUESTA]. */
enum class AuthPhase : uint8_t {
    NONE = 0,      ///< Sin autenticacion en curso.
    KEY_ENTRY,     ///< Capturando clave por teclado.
    RFID_WAIT,     ///< Esperando tarjeta RFID.
    VALIDATING,    ///< Evaluando credencial (perfil + franja).
    KEY_ROTATION   ///< Exigiendo cambio de clave por usos [RF-12].
};

/** Eventos que disparan transiciones de la FSM. */
enum class FsmEvent : uint8_t {
    NONE = 0,
    KEYPAD_KEY_PRESSED,     ///< Se presiono una tecla del matricial.
    RFID_CARD_DETECTED,     ///< Tarjeta detectada por el RC522.
    AUTH_SUCCESS,           ///< Credencial valida (segun D-01).
    AUTH_FAILURE,           ///< Credencial invalida.
    MAX_ATTEMPTS_REACHED,   ///< Contador llego a MAX_AUTH_ATTEMPTS.
    DOOR_VIOLATION,         ///< Puerta abierta fuera de estado valido.
    UNLOCK_TIMEOUT,         ///< Expiro UNLOCK_DURATION_MS.
    LOCKOUT_TIMEOUT,        ///< Expiro LOCKOUT/ALARM_DURATION_MS.
    FIRE_DETECTED,          ///< Sensor de fuego activo.
    KEY_ROTATION_REQUIRED,  ///< Clave alcanzo KEY_MAX_USES.
    RESET_SYSTEM           ///< Reposicion manual/administrativa.
};

/**
 * @class AccessFSM
 * @brief Maquina de estados finita del control de acceso.
 *
 * Responsabilidad unica: transiciones de estado y acciones asociadas
 * (desbloqueo, alarma, bloqueo). No lee sensores directamente: recibe
 * eventos ya interpretados por otros modulos (bajo acoplamiento).
 */
class AccessFSM {
public:
    /**
     * @brief Constructor.
     */
    AccessFSM();

    /**
     * @brief Inicializa la maquina en estado IDLE.
     */
    void begin();

    /**
     * @brief Procesa un evento y ejecuta la transicion resultante.
     * @param event Evento recibido de los modulos de entrada.
     */
    void dispatch(FsmEvent event);

    /**
     * @brief Actualizacion periodica (cuentas regresivas,
     *        temporizadores de bloqueo/alarma).
     *        Llamar desde la tarea task_fsm.
     */
    void update();

    /**
     * @brief Estado principal actual.
     * @return Estado FSM actual.
     */
    FsmState getState() const;

    /**
     * @brief Sub-fase de autenticacion actual.
     * @return Fase interna actual.
     */
    AuthPhase getAuthPhase() const;

    /**
     * @brief Segundos restantes de la cuenta regresiva activa.
     * @return Segundos restantes; 0 si no hay cuenta regresiva.
     */
    uint16_t getCountdownSeconds() const;

private:
    // TODO [PENDIENTE D-08]: evaluar StateMachineLib vs implementacion
    // tabular. Firmas de acciones de estado:
    void onEnterIdle();
    void onEnterErr();
    void onEnterOpen();
    void onEnterAlr();
    void onEnterKeyEntry();
    void onEnterRfidWait();
    void onEnterLockout();

    FsmState  state_;       ///< Estado principal actual.
    AuthPhase authPhase_;   ///< Sub-fase de autenticacion.
    uint8_t   failedAttempts_; ///< Intentos fallidos consecutivos.
    unsigned long stateEnterMs_; ///< Marca de entrada al estado actual.
    unsigned long countdownEndMs_; ///< Fin de cuenta regresiva (OPEN).
};

#endif /* ACCESS_FSM_H */
