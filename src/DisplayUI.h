/**
 * @file DisplayUI.h
 * @brief Interfaz de usuario en LCD 16x2 (LiquidCrystal, modo 4 bits).
 *
 * Muestra el estado FSM (IDLE/PASSWORD/OPEN/ERR/ALR), cuenta
 * regresiva, contrasena enmascarada con '*', variables ambientales,
 * PMV y estado de puerta/ventilacion.
 */

#ifndef DISPLAY_UI_H
#define DISPLAY_UI_H

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "config.h"
#include "AccessFSM.h"
#include "EnvSensors.h"
#include "ComfortController.h"

/** Datos que necesita la pantalla en cada refresco. */
struct UiSnapshot {
    FsmState      state;      ///< Estado FSM.
    uint16_t      seconds;    ///< Segundos restantes del estado.
    uint8_t       digits;     ///< Digitos ingresados (se muestran como '*').
    uint8_t       attempts;   ///< Intentos fallidos.
    AccessMessage message;    ///< Motivo de ERR/ALR.
    const char*   userName;   ///< Perfil del usuario identificado (o nullptr).
    bool          doorOpen;   ///< Estado del sensor Hall.
    EnvSnapshot   env;        ///< Variables ambientales.
    ComfortAction comfort;    ///< PMV y estado de ventilacion.
};

/**
 * @class DisplayUI
 * @brief Renderiza el estado del sistema en el LCD.
 *
 * Solo reescribe una linea cuando su contenido cambia (evita parpadeo
 * y reduce el tiempo de la tarea LCD).
 */
class DisplayUI {
public:
    /**
     * @brief Constructor.
     * @param lcd Instancia LiquidCrystal configurada con los pines.
     */
    explicit DisplayUI(LiquidCrystal& lcd);

    /**
     * @brief Inicializa el LCD y muestra la pantalla de arranque.
     */
    void begin();

    /**
     * @brief Refresca la pantalla. Llamar desde la tarea LCD.
     * @param ui Datos a mostrar.
     */
    void render(const UiSnapshot& ui);

private:
    /**
     * @brief Escribe una linea si cambio respecto a la anterior.
     * @param row Fila (0 o 1).
     * @param text Texto (se rellena/corta a LCD_COLUMNS).
     */
    void writeLine(uint8_t row, const char* text);

    /**
     * @brief Formatea la linea ambiental (T, HR, PMV, ventilacion).
     * @param out Buffer de LCD_COLUMNS+1 bytes.
     * @param ui Datos a mostrar.
     */
    static void formatEnvLine(char* out, const UiSnapshot& ui);

    LiquidCrystal& lcd_;                          ///< LCD 16x2.
    char lines_[LCD_ROWS][LCD_COLUMNS + 1];       ///< Contenido mostrado.
};

#endif /* DISPLAY_UI_H */
