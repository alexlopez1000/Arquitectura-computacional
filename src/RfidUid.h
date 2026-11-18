/**
 * @file RfidUid.h
 * @brief UID de tarjeta RFID de 4 bytes (MIFARE Classic) o 7 bytes
 *        (MIFARE Ultralight/NTAG) y su comparacion.
 *
 * Tipo compartido por RfidReader (productor) y UserManager
 * (persistencia) sin acoplar UserManager al driver MFRC522.
 */

#ifndef RFID_UID_H
#define RFID_UID_H

#include <Arduino.h>
#include "config.h"

/** UID de una tarjeta RFID; los bytes a partir de @c size valen 0x00. */
struct RfidUid {
    uint8_t bytes[RFID_UID_MAX_SIZE]; ///< Bytes del UID (relleno con 0x00).
    uint8_t size;                     ///< Longitud real: 4 o 7.
};

/**
 * @brief Indica si la longitud del UID es soportada.
 * @param uid UID a revisar.
 * @return true si @c uid.size es 4 o 7.
 */
inline bool rfidUidIsValid(const RfidUid& uid) {
    return uid.size == RFID_UID_SIZE_SINGLE || uid.size == RFID_UID_SIZE_DOUBLE;
}

/**
 * @brief Compara dos UID tratando los bytes posteriores a @c size como 0x00.
 * @param a Primer UID.
 * @param b Segundo UID.
 * @return true si ambos son validos, tienen la misma longitud y los mismos bytes.
 */
inline bool rfidUidEquals(const RfidUid& a, const RfidUid& b) {
    if (!rfidUidIsValid(a) || a.size != b.size) return false;
    for (uint8_t i = 0; i < RFID_UID_MAX_SIZE; i++) {
        const uint8_t ba = (i < a.size) ? a.bytes[i] : 0x00;
        const uint8_t bb = (i < b.size) ? b.bytes[i] : 0x00;
        if (ba != bb) return false;
    }
    return true;
}

#endif /* RFID_UID_H */
