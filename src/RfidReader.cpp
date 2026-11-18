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
    memset(lastUid_.bytes, 0, sizeof(lastUid_.bytes));
    const uint8_t n = mfrc522_.uid.size < sizeof(lastUid_.bytes)
                          ? mfrc522_.uid.size : sizeof(lastUid_.bytes);
    memcpy(lastUid_.bytes, mfrc522_.uid.uidByte, n);
    mfrc522_.PICC_HaltA();
    mfrc522_.PCD_StopCrypto1();
    return true;
}

RfidUid RfidReader::lastUid() const { return lastUid_; }
