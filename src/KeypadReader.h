/**
 * @file KeypadReader.h
 * @brief Lectura no bloqueante del teclado matricial 4x4.
 *
 * [ESTANDAR DE CLASE] La clase 10 del curso describe el escaneo por
 * bitmask de puerto (filas como salidas, columnas como entradas).
 * La implementacion definitiva seguira ese patron.
 */

#ifndef KEYPAD_READER_H
#define KEYPAD_READER_H

#include <Arduino.h>

/** Tecla presionada; 0 = ninguna. */
using KeyCode = char;

/**
 * @class KeypadReader
 * @brief Escanea el teclado matricial y publica teclas con
 *        anti-rebote por tiempo.
 *
 * Responsabilidad unica: entrada de teclado. No interpreta la clave
 * ni decide transiciones; entrega eventos a AuthManager/FSM.
 */
class KeypadReader {
public:
    /**
     * @brief Constructor.
     */
    KeypadReader();

    /**
     * @brief Configura pines del teclado segun config.h.
     */
    void begin();

    /**
     * @brief Debounce/update periodico del escaneo.
     *        Llamar desde task_input_keypad.
     */
    void update();

    /**
     * @brief Ultima tecla detectada (consumo por evento).
     * @return Tecla presionada o 0 si no hay evento nuevo.
     */
    KeyCode readKey();

private:
    KeyCode  lastKey_;         ///< Ultima tecla leida.
    unsigned long lastScanMs_; ///< Marca del ultimo escaneo.
    // TODO [PENDIENTE D-05]: mapear mapa de teclas real del teclado
    // (filas/columnas -> caracteres) segun pinout confirmado.
};

#endif /* KEYPAD_READER_H */
