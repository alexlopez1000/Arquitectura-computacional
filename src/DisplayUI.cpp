/**
 * @file DisplayUI.cpp
 * @brief Implementacion de la interfaz LCD.
 */

#include "DisplayUI.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

DisplayUI::DisplayUI(LiquidCrystal& lcd) : lcd_(lcd), lines_() {}

void DisplayUI::begin() {
    lcd_.begin(LCD_COLUMNS, LCD_ROWS);
    lcd_.clear();
    writeLine(0, "ARQC Acceso 2FA");
    writeLine(1, "Iniciando...");
}

void DisplayUI::writeLine(uint8_t row, const char* text) {
    char padded[LCD_COLUMNS + 1];
    snprintf(padded, sizeof(padded), "%-16.16s", text);
    if (strcmp(padded, lines_[row]) == 0) return;
    memcpy(lines_[row], padded, sizeof(padded));
    lcd_.setCursor(0, row);
    lcd_.print(padded);
}

void DisplayUI::formatEnvLine(char* out, const UiSnapshot& ui) {
    if (!ui.comfort.valid) {
        snprintf(out, LCD_COLUMNS + 1, "DHT11 sin datos");
        return;
    }
    int pmv10 = static_cast<int>(lroundf(ui.comfort.pmv * 10.0f));
    if (pmv10 > 99) pmv10 = 99;
    if (pmv10 < -99) pmv10 = -99;
    const char sign = pmv10 < 0 ? '-' : '+';
    const int mag = abs(pmv10);
    snprintf(out, LCD_COLUMNS + 1, "T%02d H%02d PMV%c%d.%d%c",
             static_cast<int>(lroundf(ui.env.temperatureC)),
             static_cast<int>(lroundf(ui.env.humidityPct)),
             sign, mag / 10, mag % 10, ui.comfort.fanOn ? 'V' : ' ');
}

void DisplayUI::render(const UiSnapshot& ui) {
    char l1[LCD_COLUMNS + 1];
    char l2[LCD_COLUMNS + 1];

    switch (ui.state) {
        case FsmState::IDLE:
            snprintf(l1, sizeof(l1), "IDLE  Puerta:%s", ui.doorOpen ? "ABI" : "CER");
            formatEnvLine(l2, ui);
            break;

        case FsmState::PASSWORD: {
            snprintf(l1, sizeof(l1), "PASSWORD    %3us", ui.seconds);
            char stars[MAX_PASSWORD_LENGTH + 1];
            memset(stars, '*', ui.digits);
            stars[ui.digits] = '\0';
            snprintf(l2, sizeof(l2), "Clave:%s", stars);
            break;
        }

        case FsmState::OPEN:
            snprintf(l1, sizeof(l1), "OPEN  Bienvenido");
            snprintf(l2, sizeof(l2), "%-11.11s %3us",
                     ui.userName ? ui.userName : "", ui.seconds);
            break;

        case FsmState::ERR:
            snprintf(l1, sizeof(l1), "ERR  Intento %u/%u", ui.attempts, MAX_AUTH_ATTEMPTS);
            snprintf(l2, sizeof(l2), "%s", ui.message == AccessMessage::CARD_UNKNOWN
                                               ? "Tarjeta invalida" : "Clave incorrecta");
            break;

        case FsmState::ALR:
        default:
            snprintf(l1, sizeof(l1), "ALR   BLOQUEADO");
            snprintf(l2, sizeof(l2), "Espere %3us", ui.seconds);
            break;
    }

    writeLine(0, l1);
    writeLine(1, l2);
}
