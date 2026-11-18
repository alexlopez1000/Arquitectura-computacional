/**
 * @file RfidReader.cpp
 * @brief Implementacion del lector RFID MFRC522.
 */

#include "RfidReader.h"
#include <SPI.h>
#include <string.h>

RfidReader::RfidReader(uint8_t ssPin, uint8_t rstPin)
    : mfrc522_(ssPin, rstPin), lastUid_() {}

void RfidReader::begin() {
    SPI.begin();
    mfrc522_.PCD_Init();
}

bool RfidReader::poll() {
    if (!mfrc522_.PICC_IsNewCardPresent() || !mfrc522_.PICC_ReadCardSerial()) {
        return false;
    }
    RfidUid uid;
    memset(&uid, 0, sizeof(uid));
    uid.size = mfrc522_.uid.size;
    const bool valid = rfidUidIsValid(uid);
    if (valid) {
        memcpy(uid.bytes, mfrc522_.uid.uidByte, uid.size);
        lastUid_ = uid;
    }
    mfrc522_.PICC_HaltA();
    mfrc522_.PCD_StopCrypto1();
    return valid;
}

RfidUid RfidReader::lastUid() const { return lastUid_; }
