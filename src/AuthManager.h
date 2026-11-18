/**
 * @file AuthManager.h
 * @brief Validacion de credenciales 2FA (RFID + contrasena) y
 *        contador de intentos fallidos.
 */

#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <Arduino.h>
#include "config.h"
#include "UserManager.h"

/**
 * @class AuthManager
 * @brief Aplica las reglas de autenticacion sobre UserManager.
 *
 * Factor 1: UID RFID registrado. Factor 2: contrasena individual del
 * mismo usuario. Cada fallo (tarjeta desconocida o contrasena
 * incorrecta) cuenta como intento; al llegar a MAX_AUTH_ATTEMPTS la
 * FSM pasa a ALR.
 */
class AuthManager {
public:
    /**
     * @brief Constructor.
     * @param users Repositorio de usuarios inyectado.
     */
    explicit AuthManager(UserManager& users);

    /**
     * @brief Factor 1: valida la tarjeta presentada.
     * @param uid UID leido (4 o 7 bytes).
     * @return Indice del usuario o -1 si la tarjeta no esta registrada.
     */
    int8_t validateCard(const RfidUid& uid) const;

    /**
     * @brief Factor 2: valida la contrasena del usuario ya identificado.
     * @param userIndex Usuario identificado por la tarjeta.
     * @param password Contrasena digitada.
     * @return true si es correcta.
     */
    bool validatePassword(int8_t userIndex, const char* password) const;

    /**
     * @brief Registra un intento fallido.
     * @return true si se alcanzo MAX_AUTH_ATTEMPTS.
     */
    bool registerFailure();

    /**
     * @brief Reinicia el contador de intentos fallidos.
     */
    void resetAttempts();

    /**
     * @brief Intentos fallidos consecutivos.
     * @return Contador actual.
     */
    uint8_t failedAttempts() const;

    /**
     * @brief Acceso de solo lectura al repositorio de usuarios.
     * @return Referencia a UserManager.
     */
    const UserManager& users() const;

private:
    UserManager& users_;       ///< Repositorio de usuarios.
    uint8_t failedAttempts_;   ///< Fallos consecutivos.
};

#endif /* AUTH_MANAGER_H */
