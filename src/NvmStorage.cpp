/**
 * @file NvmStorage.cpp
 * @brief Implementacion de la persistencia EEPROM con version y CRC.
 */

#include "NvmStorage.h"
#include <EEPROM.h>
#include "config.h"

namespace {
/** Cabecera de la imagen persistida. */
struct __attribute__((packed)) ImageHeader {
    uint16_t magic;   ///< Firma EEPROM_MAGIC.
    uint8_t  version; ///< Version del esquema.
    uint16_t length;  ///< Longitud de la carga util.
};
} // namespace

NvmStorage::NvmStorage(uint16_t baseAddr) : baseAddr_(baseAddr) {}

uint16_t NvmStorage::crc16(uint16_t crc, const uint8_t* data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (uint8_t b = 0; b < 8; b++) {
            crc = (crc & 0x8000U) ? static_cast<uint16_t>((crc << 1) ^ 0x1021U)
                                  : static_cast<uint16_t>(crc << 1);
        }
    }
    return crc;
}

bool NvmStorage::load(void* data, uint16_t len) const {
    ImageHeader hdr;
    EEPROM.get(baseAddr_, hdr);
    if (hdr.magic != EEPROM_MAGIC || hdr.version != EEPROM_LAYOUT_VERSION ||
        hdr.length != len) {
        return false;
    }
    if (baseAddr_ + sizeof(hdr) + len + sizeof(uint16_t) > EEPROM.length()) {
        return false;
    }

    uint8_t* out = static_cast<uint8_t*>(data);
    uint16_t addr = baseAddr_ + sizeof(hdr);
    for (uint16_t i = 0; i < len; i++) {
        out[i] = EEPROM.read(addr + i);
    }
    uint16_t stored;
    EEPROM.get(addr + len, stored);

    uint16_t crc = crc16(0xFFFFU, &hdr.version, sizeof(hdr) - sizeof(hdr.magic));
    crc = crc16(crc, out, len);
    return crc == stored;
}

bool NvmStorage::save(const void* data, uint16_t len) {
    if (baseAddr_ + sizeof(ImageHeader) + len + sizeof(uint16_t) > EEPROM.length()) {
        return false;
    }
    ImageHeader hdr = { EEPROM_MAGIC, EEPROM_LAYOUT_VERSION, len };
    const uint8_t* in = static_cast<const uint8_t*>(data);

    uint16_t crc = crc16(0xFFFFU, &hdr.version, sizeof(hdr) - sizeof(hdr.magic));
    crc = crc16(crc, in, len);

    uint16_t addr = baseAddr_ + sizeof(hdr);
    for (uint16_t i = 0; i < len; i++) {
        EEPROM.update(addr + i, in[i]);
    }
    EEPROM.put(addr + len, crc);
    EEPROM.put(baseAddr_, hdr);

    for (uint16_t i = 0; i < len; i++) {
        if (EEPROM.read(addr + i) != in[i]) return false;
    }
    return true;
}
