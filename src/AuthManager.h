/**
 * @file AuthManager.h
 * @brief Gestor de autenticacion multicriterio (RFID + teclado).
 *
 * Resuelve la decision D-01 (doble factor vs metodos independientes
 * por perfil) mediante configuracion por perfil: cada perfil define
 * que credenciales acepta.
 *
 * TODO [PENDIENTE D-01]: confirmar con el profesor la regla exacta
 * de combinacion RFID/clave. Mientras tanto, la matriz de decision
 * es configurable en config.h.
 */

#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <Arduino.h>
#include "config.h"

/** Resultado de un intento de autenticacion. */
enum class AuthResult : uint8_t {
    GRANTED = 0,        ///< Acceso autorizado.
    DENIED_CREDENTIAL,  ///< Credencial incorrecta.
    DENIED_SCHEDULE,    ///< Fuera de franja horaria [RF-05].
    DENIED_PROFILE,     ///< Perfil no permite este metodo.
    LOCKED_OUT          ///< Sistema en bloqueo temporal.
};

/** Credencial presentada por el usuario (origen RFID o teclado). */
struct Credential {
    bool        isRfid;      ///< true si proviene de tarjeta RFID.
    uint8_t     uid[4];      ///< NUID de 4 bytes si es RFID.
    char        key[MAX_KEY_LENGTH + 1]; ///< Clave numerica si es teclado.
};

/**
 * @class AuthManager
 * @brief Autenticacion de usuarios segun perfil y franja horaria.
 *
 * Responsabilidad unica: validar credenciales contra UserManager y
 * ScheduleValidator, y reportar el resultado a la FSM.
 * No conoce pines ni protocolo de comunicacion de los lectores.
 */
class AuthManager {
public:
    /**
     * @brief Constructor.
     */
    AuthManager();

    /**
     * @brief Inicializa dependencias del gestor.
     */
    void begin();

    /**
     * @brief Valida una credencial presentada.
     * @param cred Credencial a validar.
     * @return Resultado de la autenticacion.
     */
    AuthResult authenticate(const Credential& cred);

    /**
     * @brief Registra un intento fallido y evalua bloqueo.
     * @return true si se alcanzo MAX_AUTH_ATTEMPTS.
     */
    bool registerFailure();

    /**
     * @brief Reinicia el contador de intentos fallidos.
     */
    void resetAttempts();

    /**
     * @brief Intentos fallidos consecutivos actuales.
     * @return Contador de intentos.
     */
    uint8_t getFailedAttempts() const;

private:
    uint8_t failedAttempts_; ///< Contador de fallos consecutivos.
    // TODO [PENDIENTE D-01]: punteros a UserManager y
    // ScheduleValidator se inyectaran en begin()/setup segun
    // la combinacion RFID/clave confirmada.
};

#endif /* AUTH_MANAGER_H */
