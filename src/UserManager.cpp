/**
 * @file UserManager.cpp
 * @brief Implementacion del repositorio de usuarios.
 */

#include "UserManager.h"
#include <string.h>

namespace {
/**
 * Usuarios por defecto, alineados con las tarjetas predefinidas del
 * MFRC522 de Wokwi (Blue Card, Green Card y NFC Tag de 7 bytes).
 * Para hardware real, reemplazar los UID por los de las tarjetas
 * (el UID de cualquier tarjeta leida se imprime por Serial) e
 * incrementar EEPROM_LAYOUT_VERSION para forzar su reescritura.
 */
const UserRecord kDefaultUsers[NUM_USERS] PROGMEM = {
    { { {0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00}, 4 }, Profile::SECURITY, "1234" },
    { { {0x11, 0x22, 0x33, 0x44, 0x00, 0x00, 0x00}, 4 }, Profile::OPERATOR, "5678" },
    { { {0x04, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66}, 7 }, Profile::MANAGER,  "2468" },
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

int8_t UserManager::findByUid(const RfidUid& uid) const {
    for (int8_t i = 0; i < NUM_USERS; i++) {
        if (rfidUidEquals(users_[i].uid, uid)) {
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
