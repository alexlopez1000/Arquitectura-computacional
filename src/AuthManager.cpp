/**
 * @file AuthManager.cpp
 * @brief Implementacion del gestor de autenticacion 2FA.
 */

#include "AuthManager.h"

AuthManager::AuthManager(UserManager& users) : users_(users), failedAttempts_(0) {}

int8_t AuthManager::validateCard(const RfidUid& uid) const {
    return users_.findByUid(uid);
}

bool AuthManager::validatePassword(int8_t userIndex, const char* password) const {
    return users_.checkPassword(userIndex, password);
}

bool AuthManager::registerFailure() {
    if (failedAttempts_ < MAX_AUTH_ATTEMPTS) failedAttempts_++;
    return failedAttempts_ >= MAX_AUTH_ATTEMPTS;
}

void AuthManager::resetAttempts() { failedAttempts_ = 0; }

uint8_t AuthManager::failedAttempts() const { return failedAttempts_; }

const UserManager& AuthManager::users() const { return users_; }
