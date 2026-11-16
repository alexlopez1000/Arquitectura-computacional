/**
 * @file AuthManager.cpp
 * @brief Implementacion del gestor de autenticacion.
 */

#include "AuthManager.h"

AuthManager::AuthManager() : failedAttempts_(0) {}

void AuthManager::begin() {
    // TODO [PENDIENTE D-01]: inyectar UserManager/ScheduleValidator.
}

AuthResult AuthManager::authenticate(const Credential& cred) {
    // TODO [PENDIENTE D-01]: regla de combinacion RFID/clave.
    return AuthResult::DENIED_CREDENTIAL;
}

bool AuthManager::registerFailure() {
    // TODO: failedAttempts_++ y comparar con MAX_AUTH_ATTEMPTS.
    return false;
}

void AuthManager::resetAttempts() { failedAttempts_ = 0; }
uint8_t AuthManager::getFailedAttempts() const { return failedAttempts_; }
