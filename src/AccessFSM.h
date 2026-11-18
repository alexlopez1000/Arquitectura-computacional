/**
 * @file AccessFSM.h
 * @brief Maquina de estados del control de acceso con autenticacion
 *        2FA secuencial obligatoria, implementada con StateMachineLib.
 *
 * Estados (visibles en el LCD):
 *  - IDLE     : espera tarjeta RFID.
 *  - PASSWORD : tarjeta valida (RFID_OK); se digita la contrasena
 *               individual del usuario, enmascarada con '*'.
 *  - OPEN     : contrasena correcta; LED verde + buzzer durante
 *               OPEN_DURATION_MS y regreso a IDLE.
 *  - ERR      : credencial incorrecta con menos de 3 intentos;
 *               mensaje en LCD durante ERR_DISPLAY_MS y regreso a IDLE.
 *  - ALR      : 3 intentos fallidos; LED rojo + buzzer y bloqueo
 *               temporal de LOCKOUT_DURATION_MS.
 *
 * Tabla de transiciones:
 *
 * | Origen   | Condicion                         | Destino  |
 * |----------|-----------------------------------|----------|
 * | IDLE     | UID registrado                    | PASSWORD |
 * | IDLE     | UID desconocido, intentos < 3     | ERR      |
 * | IDLE     | UID desconocido, intentos = 3     | ALR      |
 * | PASSWORD | Contrasena correcta               | OPEN     |
 * | PASSWORD | Contrasena incorrecta, < 3        | ERR      |
 * | PASSWORD | Contrasena incorrecta, = 3        | ALR      |
 * | PASSWORD | Tecla cancelar o tiempo agotado   | IDLE     |
 * | OPEN     | OPEN_DURATION_MS cumplido         | IDLE     |
 * | ERR      | ERR_DISPLAY_MS cumplido           | IDLE     |
 * | ALR      | LOCKOUT_DURATION_MS cumplido      | IDLE     |
 *
 * Todos los tiempos se miden con millis() y resta sin signo
 * (uint32_t), correcta aun cuando millis() desborda (~49.7 dias).
 */

#ifndef ACCESS_FSM_H
#define ACCESS_FSM_H

#include <Arduino.h>
#include <StateMachineLib.h>
#include "config.h"
#include "AuthManager.h"
#include "Actuators.h"
#include "KeypadReader.h"
#include "RfidReader.h"

/** Estados de la FSM de acceso (indices de StateMachineLib). */
enum class FsmState : uint8_t {
    IDLE     = 0, ///< Espera tarjeta RFID.
    PASSWORD = 1, ///< RFID_OK: captura de contrasena.
    OPEN     = 2, ///< Acceso permitido.
    ERR      = 3, ///< Credencial incorrecta.
    ALR      = 4  ///< Alarma y bloqueo temporal.
};

/** Motivo mostrado en el LCD para ERR/ALR. */
enum class AccessMessage : uint8_t {
    NONE = 0,        ///< Sin mensaje.
    CARD_UNKNOWN,    ///< Tarjeta no registrada.
    WRONG_PASSWORD   ///< Contrasena incorrecta.
};

/**
 * @class AccessFSM
 * @brief FSM de acceso 2FA sobre StateMachineLib.
 *
 * Recibe eventos ya interpretados (tarjeta leida, tecla presionada),
 * los convierte en una entrada interna y deja que StateMachineLib
 * evalue las condiciones de transicion en update(). Las acciones de
 * entrada de cada estado ordenan la senalizacion a Actuators.
 *
 * @note StateMachineLib usa punteros a funcion sin contexto; por eso
 *       las condiciones y acciones son estaticas y acceden a la unica
 *       instancia mediante self_.
 */
class AccessFSM {
public:
    /**
     * @brief Constructor.
     * @param auth Gestor de autenticacion.
     * @param actuators Actuadores de senalizacion.
     */
    AccessFSM(AuthManager& auth, Actuators& actuators);

    /**
     * @brief Registra transiciones/acciones y entra en IDLE.
     */
    void begin();

    /**
     * @brief Evento de tarjeta RFID leida (solo se procesa en IDLE).
     * @param uid UID leido (4 o 7 bytes).
     */
    void onCardDetected(const RfidUid& uid);

    /**
     * @brief Evento de tecla (solo se procesa en PASSWORD).
     *        Digitos: se agregan; KEY_DELETE borra; KEY_ENTER valida;
     *        KEY_CANCEL aborta.
     * @param key Tecla presionada.
     */
    void onKey(KeyCode key);

    /**
     * @brief Evalua temporizadores y transiciones. Llamar desde la tarea FSM.
     */
    void update();

    /**
     * @brief Estado actual.
     * @return Estado FSM.
     */
    FsmState state() const;

    /**
     * @brief Digitos ingresados (para mostrar '*').
     * @return Numero de digitos en el buffer.
     */
    uint8_t enteredDigits() const;

    /**
     * @brief Segundos restantes del temporizador del estado actual.
     * @return Segundos (redondeo hacia arriba); 0 si el estado no tiene temporizador.
     */
    uint16_t remainingSeconds() const;

    /**
     * @brief Intentos fallidos consecutivos.
     * @return Contador de AuthManager.
     */
    uint8_t failedAttempts() const;

    /**
     * @brief Usuario identificado por la tarjeta.
     * @return Indice del usuario o -1.
     */
    int8_t currentUser() const;

    /**
     * @brief Motivo del ultimo ERR/ALR.
     * @return Mensaje para el LCD.
     */
    AccessMessage lastMessage() const;

private:
    /** Entrada interna que consumen las condiciones de transicion. */
    enum class Input : uint8_t {
        NONE = 0,     ///< Sin entrada pendiente.
        CARD_OK,      ///< Factor 1 valido.
        PASSWORD_OK,  ///< Factor 2 valido.
        FAIL,         ///< Credencial incorrecta, intentos < 3.
        FAIL_LIMIT,   ///< Credencial incorrecta, intentos = 3.
        ABORT         ///< Cancelacion o tiempo agotado.
    };

    /**
     * @brief Registra un fallo y genera FAIL o FAIL_LIMIT.
     * @param msg Motivo a mostrar.
     */
    void registerFailure(AccessMessage msg);

    /**
     * @brief Duracion del temporizador del estado actual.
     * @return Duracion en ms (0 si no aplica).
     */
    uint32_t stateDurationMs() const;

    /**
     * @brief Indica si vencio el temporizador del estado actual.
     * @return true si millis() - entrada >= duracion.
     */
    bool stateTimedOut() const;

    /**
     * @brief Limpia el buffer de contrasena.
     */
    void clearPassword();

    /**
     * @brief Marca el instante de entrada al estado.
     */
    void markEntry();

    /** @brief Condicion: tarjeta valida. @return true si se cumple. */
    static bool condCardOk();
    /** @brief Condicion: contrasena correcta. @return true si se cumple. */
    static bool condPasswordOk();
    /** @brief Condicion: fallo con intentos < 3. @return true si se cumple. */
    static bool condFail();
    /** @brief Condicion: fallo con intentos = 3. @return true si se cumple. */
    static bool condFailLimit();
    /** @brief Condicion: cancelacion o tiempo agotado. @return true si se cumple. */
    static bool condAbort();
    /** @brief Condicion: vencio el temporizador del estado. @return true si se cumple. */
    static bool condTimeout();

    /** @brief Accion de entrada a IDLE. */
    static void enterIdle();
    /** @brief Accion de entrada a PASSWORD. */
    static void enterPassword();
    /** @brief Accion de entrada a OPEN. */
    static void enterOpen();
    /** @brief Accion de entrada a ERR. */
    static void enterErr();
    /** @brief Accion de entrada a ALR. */
    static void enterAlr();
    /** @brief Accion de salida de ALR. */
    static void leaveAlr();

    static AccessFSM* self_;      ///< Instancia unica usada por los callbacks.

    AuthManager& auth_;           ///< Validacion de credenciales.
    Actuators&   actuators_;      ///< Senalizacion LED/buzzer.
    StateMachine machine_;        ///< Motor StateMachineLib.
    Input        input_;          ///< Entrada pendiente.
    AccessMessage message_;       ///< Motivo del ultimo fallo.
    int8_t       user_;           ///< Usuario identificado (-1 = ninguno).
    char         password_[MAX_PASSWORD_LENGTH + 1]; ///< Buffer de digitos.
    uint8_t      length_;         ///< Digitos en el buffer.
    uint32_t     stateEnterMs_;   ///< millis() al entrar al estado.
};

#endif /* ACCESS_FSM_H */
