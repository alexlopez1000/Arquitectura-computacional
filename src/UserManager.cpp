/**
 * @file UserManager.cpp
 * @brief Implementacion de la gestion de usuarios y claves.
 */

#include "UserManager.h"
#include "NvmStorage.h"

UserManager::UserManager() : userCount_(0) {}

void UserManager::begin() {
    // TODO [PENDIENTE D-07]: cargar registros desde NvmStorage.
}

bool UserManager::addUser(const UserRecord& user) {
    // TODO: validar capacidad, clave >= MIN_KEY_LENGTH,
    // unicidad de clave/RFID; persistir via save().
    return false;
}

int UserManager::findByRfid(const uint8_t uid[4]) const {
    // TODO: busqueda lineal por uid.
    return -1;
}

int UserManager::findByKey(const char* key) const {
    // TODO: busqueda lineal por clave.
    return -1;
}

bool UserManager::cloneCredential(int src, int dst) {
    // TODO [RF-09]: copiar credencial; definir si copia RFID, clave o ambas.
    return false;
}

bool UserManager::registerKeyUse(int index) {
    // TODO [RF-12]: incrementar keyUses; true si >= KEY_MAX_USES.
    return false;
}

bool UserManager::changeKey(int index, const char* newKey) {
    // TODO [RF-12]: rechazar si newKey == key o == prevKey.
    return false;
}

void UserManager::save() {
    // TODO: delegar escritura a NvmStorage.
}

const UserRecord* UserManager::records() const { return users_; }
uint8_t UserManager::count() const { return userCount_; }
