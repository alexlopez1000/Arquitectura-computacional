/**
 * @file KeypadReader.cpp
 * @brief Implementacion del lector de teclado matricial.
 */

#include "KeypadReader.h"
#include "config.h"

KeypadReader::KeypadReader() : lastKey_(0), lastScanMs_(0) {}

void KeypadReader::begin() {
    // TODO: configurar filas como salidas y columnas como entradas
    // segun el patron de bitmask de puerto de la clase 10.
}

void KeypadReader::update() {
    // TODO: escaneo periodico con anti-rebote por tiempo.
}

KeyCode KeypadReader::readKey() {
    KeyCode k = lastKey_;
    lastKey_ = 0; // consumo por evento
    return k;
}
