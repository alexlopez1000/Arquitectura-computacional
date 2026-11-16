/**
 * @file RfidReader.h
 * @brief Lectura no bloqueante de tarjetas RFID (MFRC522).
 *
 * [ESTANDAR DE CLASE] Libreria MFRC522 (miguelbalboa/rfid) con SPI
 * hardware del Mega: SS=53, MOSI=51, MISO=50, SCK=52, RST=5
 * (tabla de pines del ejemplo oficial ReadNUID.ino).
 */

#ifndef RFID_READER_H
#define RFID_READER_H

#include <Arduino.h>
#include "config.h"

/** NUID de una tarjeta RFID MIFARE. */
struct RfidUid {
    uint8_t bytes[4]; ///< Identificador unico de 4 bytes.
};

/**
 * @class RfidReader
 * @brief Interfaz con el modulo MFRC522.
 *
 * Responsabilidad unica: detectar tarjetas y entregar el NUID.
 * La validacion del UID contra usuarios autorizados corresponde a
 * AuthManager/UserManager (bajo acoplamiento).
 */
class RfidReader {
public:
    /**
     * @brief Constructor.
     */
    RfidReader();

    /**
     * @brief Inicializa SPI y el PCD (lector RC522).
     */
    void begin();

    /**
     * @brief Verifica si hay una tarjeta nueva presente.
     *        Operacion potencialmente lenta: se invoca por tandas
     *        desde task_input_rfid.
     * @return true si se detecto una tarjeta nueva.
     */
    bool poll();

    /**
     * @brief NUID de la ultima tarjeta leida.
     * @return Estructura con los 4 bytes del UID.
     */
    RfidUid lastUid() const;

private:
    RfidUid lastUid_;          ///< Cache del ultimo UID leido.
    bool    cardPending_;      ///< Tarjeta detectada pendiente de lectura.
    // TODO [PENDIENTE D-01/D-05]: instancia MFRC522 rfid(SS,RST)
    // y verificacion de tipo PICC segun ejemplos del curso.
};

#endif /* RFID_READER_H */
