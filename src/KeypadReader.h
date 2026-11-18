/**
 * @file KeypadReader.h
 * @brief Lectura no bloqueante del teclado matricial 4x4.
 *
 * Escaneo por filas (salidas) y columnas (entradas con pull-up),
 * patron de la clase 10. Una tecla se publica una sola vez cuando se
 * observa estable en dos escaneos consecutivos (anti-rebote).
 */

#ifndef KEYPAD_READER_H
#define KEYPAD_READER_H

#include <Arduino.h>
#include "config.h"

/** Tecla presionada; 0 = ninguna. */
using KeyCode = char;

/**
 * @class KeypadReader
 * @brief Escanea el teclado y entrega eventos de tecla.
 *
 * Responsabilidad unica: entrada de teclado. No interpreta la
 * contrasena; la FSM consume las teclas.
 */
class KeypadReader {
public:
    /**
     * @brief Constructor.
     */
    KeypadReader();

    /**
     * @brief Configura filas como salidas (HIGH) y columnas con pull-up.
     */
    void begin();

    /**
     * @brief Escanea la matriz y aplica anti-rebote.
     *        Llamar desde la tarea del teclado.
     */
    void update();

    /**
     * @brief Consume la ultima tecla detectada.
     * @return Tecla presionada o 0 si no hay evento nuevo.
     */
    KeyCode readKey();

private:
    /**
     * @brief Recorre la matriz una vez.
     * @return Tecla presionada en este instante o 0.
     */
    KeyCode scan() const;

    KeyCode pendingKey_;  ///< Tecla pendiente de consumo.
    KeyCode lastScan_;    ///< Resultado del escaneo anterior.
    KeyCode stableKey_;   ///< Tecla estable actualmente presionada.
};

#endif /* KEYPAD_READER_H */
