/**
 * @file RfidReader.h
 * @brief Lectura de tarjetas RFID con el modulo MFRC522 (SPI).
 *
 * SPI hardware del Mega: SS=53, MOSI=51, MISO=50, SCK=52, RST=5
 * (tabla de pines del ejemplo oficial ReadNUID.ino).
 */

#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include <MFRC522.h>
#include "config.h"
#include "RfidUid.h"

/**
 * @class RfidReader
 * @brief Detecta tarjetas nuevas y entrega su UID (4 o 7 bytes).
 *
 * La validacion del UID corresponde a AuthManager (bajo acoplamiento).
 */
class RfidReader {
public:
    /**
     * @brief Constructor.
     * @param ssPin Pin SS (SDA) del RC522.
     * @param rstPin Pin RST del RC522.
     */
    RfidReader(uint8_t ssPin, uint8_t rstPin);

    /**
     * @brief Inicializa el bus SPI y el lector RC522.
     */
    void begin();

    /**
     * @brief Comprueba si hay una tarjeta nueva y lee su UID.
     *        Llamar desde la tarea RFID. Las tarjetas con UID distinto
     *        de 4 o 7 bytes se descartan.
     * @return true si se leyo una tarjeta nueva con UID soportado.
     */
    bool poll();

    /**
     * @brief UID de la ultima tarjeta leida.
     * @return Copia del UID.
     */
    RfidUid lastUid() const;

private:
    MFRC522 mfrc522_; ///< Driver del lector.
    RfidUid lastUid_; ///< Ultimo UID leido.
};

#endif /* RFID_READER_H */
