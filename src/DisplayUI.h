/**
 * @file DisplayUI.h
 * @brief Interfaz de usuario en LCD 16x2.
 *
 * Requisito [CONFIRMADO: RF-14]: mostrar estado FSM (IDLE/ERR/
 * OPEN/ALR), conteo regresivo, digitos enmascarados, variables
 * ambientales y mensajes.
 *
 * [ESTANDAR DE CLASE] LCD16x2 con LiquidCrystal (modo 4 bits,
 * pines RS/EN/D4-D7 definidos en config.h).
 */

#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "AccessFSM.h"
#include "EnvSensors.h"

/**
 * @class DisplayUI
 * @brief Renderiza el estado del sistema en el LCD.
 *
 * Responsabilidad unica: presentacion. No decide estados ni
 * ejecuta logica de negocio; solo refleja snapshots.
 */
class DisplayUI {
public:
    /**
     * @brief Constructor.
     * @param lcd Instancia LiquidCrystal configurada con pines.
     */
    explicit DisplayUI(LiquidCrystal& lcd);

    /**
     * @brief Inicializa la pantalla (setup + mensaje de arranque).
     */
    void begin();

    /**
     * @brief Refresca la pantalla segun estado actual.
     * @param state Estado FSM actual.
     * @param countdown Segundos restantes de cuenta regresiva.
     * @param env Variables ambientales a mostrar.
     * @param maskedDigits Digitos enmascarados de la clave en curso.
     */
    void refresh(FsmState state,
                 uint16_t countdown,
                 const EnvSnapshot& env,
                 const char* maskedDigits);

    /**
     * @brief Muestra un mensaje transitorio en la linea inferior.
     * @param message Texto a mostrar (max 16 chars).
     */
    void showMessage(const char* message);

private:
    LiquidCrystal& lcd_; ///< Referencia al LCD 16x2.
    // TODO: layout exacto de lineas (estado+cuenta regresiva en
    // linea 1; ambiente+mensaje en linea 2) segun pruebas visuales.
};

#endif /* DISPLAY_UI_H */
