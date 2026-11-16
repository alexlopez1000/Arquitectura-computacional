/**
 * @file RfidReader.cpp
 * @brief Implementacion del lector RFID MFRC522.
 */

#include "RfidReader.h"
#include <SPI.h>
#include <MFRC522.h>

RfidReader::RfidReader() : cardPending_(false) {}

void RfidReader::begin() {
    // TODO: SPI.begin(); rfid.PCD_Init();
    // Pines: SS=PIN_RFID_SS, RST=PIN_RFID_RST (config.h).
}

bool RfidReader::poll() {
    // TODO: PICC_IsNewCardPresent + PICC_ReadCardSerial
    // (ejemplos ReadNUID.ino del curso).
    return false;
}

RfidUid RfidReader::lastUid() const { return lastUid_; }
