/**
 * @file NvmStorage.cpp
 * @brief Implementacion del almacenamiento EEPROM.
 */

#include "NvmStorage.h"
#include <EEPROM.h>

NvmStorage::NvmStorage() {}

void NvmStorage::begin() {
    // TODO [PENDIENTE D-07]: verificar magic number; inicializar
    // valores por defecto si la NVM esta vacia.
}

bool NvmStorage::isValid() const {
    // TODO: leer magic + CRC.
    return false;
}

bool NvmStorage::writeBlock(uint16_t addr, const void* data, uint16_t len) {
    // TODO: EEPROM.write por bytes + verificacion.
    return false;
}

bool NvmStorage::readBlock(uint16_t addr, void* data, uint16_t len) const {
    // TODO: EEPROM.read por bytes + validacion CRC.
    return false;
}
