/**
 * @file DisplayUI.cpp
 * @brief Implementacion de la interfaz LCD.
 */

#include "DisplayUI.h"
#include "config.h"

DisplayUI::DisplayUI(LiquidCrystal& lcd) : lcd_(lcd) {}

void DisplayUI::begin() {
    // TODO: lcd_.begin(LCD_COLUMNS, LCD_ROWS);
    // pantalla inicial de arranque.
}

void DisplayUI::refresh(FsmState state, uint16_t countdown,
                        const EnvSnapshot& env,
                        const char* maskedDigits) {
    // TODO [RF-14]: linea 1 = estado + cuenta regresiva;
    // linea 2 = T°/HR + mensaje. Digitos siempre enmascarados '*'.
}

void DisplayUI::showMessage(const char* message) {
    // TODO: escribir mensaje en linea disponible (max 16 chars).
}
