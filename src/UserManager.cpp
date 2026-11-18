/**
 * @file UserManager.cpp
 * @brief Implementacion del repositorio de usuarios.
 */

#include "UserManager.h"
#include <string.h>

namespace {
/**
 * Usuarios por defecto. Reemplazar los UID por los de las tarjetas
 * reales (el UID de cualquier tarjeta leida se imprime por Serial) e
 * incrementar EEPROM_LAYOUT_VERSION para forzar su reescritura.
 */
const UserRecord kDefaultUsers[NUM_USERS] PROGMEM = {
    { {0xA1, 0xB2, 0xC3, 0xD4}, Profile::SECURITY, "1234" },
    { {0x11, 0x22, 0x33, 0x44}, Profile::OPERATOR, "5678" },
    { {0xDE, 0xAD, 0xBE, 0xEF}, Profile::MANAGER,  "2468" },
};

bool isValidPassword(const char* pwd) {
    size_t len = strlen(pwd);
    if (len < MIN_PASSWORD_LENGTH || len > MAX_PASSWORD_LENGTH) return false;
    for (size_t i = 0; i < len; i++) {
        if (pwd[i] < '0' || pwd[i] > '9') return false;
    }
    return true;
}
} // namespace

UserManager::UserManager(NvmStorage& storage) : storage_(storage), users_() {}

bool UserManager::begin() {
    if (storage_.load(users_, sizeof(users_))) {
        return true;
    }
    restoreDefaults();
    return false;
}

void UserManager::restoreDefaults() {
    memcpy_P(users_, kDefaultUsers, sizeof(users_));
    storage_.save(users_, sizeof(users_));
}

int8_t UserManager::findByUid(const uint8_t uid[4]) const {
    for (int8_t i = 0; i < NUM_USERS; i++) {
        if (memcmp(users_[i].rfidUid, uid, sizeof(users_[i].rfidUid)) == 0) {
            return i;
        }
    }
    return -1;
}

bool UserManager::checkPassword(int8_t index, const char* password) const {
    if (index < 0 || index >= NUM_USERS) return false;
    return strncmp(users_[index].password, password, MAX_PASSWORD_LENGTH + 1) == 0;
}

bool UserManager::setPassword(int8_t index, const char* newPassword) {
    if (index < 0 || index >= NUM_USERS || !isValidPassword(newPassword)) {
        return false;
    }
    strncpy(users_[index].password, newPassword, MAX_PASSWORD_LENGTH);
    users_[index].password[MAX_PASSWORD_LENGTH] = '\0';
    return storage_.save(users_, sizeof(users_));
}

Profile UserManager::profileOf(int8_t index) const {
    return users_[(index >= 0 && index < NUM_USERS) ? index : 0].profile;
}

const char* UserManager::profileName(Profile profile) {
    switch (profile) {
        case Profile::SECURITY:    return "SEGURIDAD";
        case Profile::OPERATOR:    return "OPERARIO";
        case Profile::COORDINATOR: return "COORDINADOR";
        case Profile::MANAGER:     return "GERENTE";
    }
    return "?";
}
