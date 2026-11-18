/**
 * @file KeypadReader.cpp
 * @brief Implementacion del lector de teclado matricial.
 */

#include "KeypadReader.h"

namespace {
const uint8_t kRowPins[KEYPAD_NUM_ROWS] = PIN_KEYPAD_ROWS;
const uint8_t kColPins[KEYPAD_NUM_COLS] = PIN_KEYPAD_COLS;
const char kKeyMap[KEYPAD_NUM_ROWS][KEYPAD_NUM_COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'},
};
} // namespace

KeypadReader::KeypadReader() : pendingKey_(0), lastScan_(0), stableKey_(0) {}

void KeypadReader::begin() {
    for (uint8_t r = 0; r < KEYPAD_NUM_ROWS; r++) {
        pinMode(kRowPins[r], OUTPUT);
        digitalWrite(kRowPins[r], HIGH);
    }
    for (uint8_t c = 0; c < KEYPAD_NUM_COLS; c++) {
        pinMode(kColPins[c], INPUT_PULLUP);
    }
}

KeyCode KeypadReader::scan() const {
    KeyCode key = 0;
    for (uint8_t r = 0; r < KEYPAD_NUM_ROWS && key == 0; r++) {
        digitalWrite(kRowPins[r], LOW);
        delayMicroseconds(5);  // asentamiento de la linea
        uint8_t colMask = 0;
        for (uint8_t c = 0; c < KEYPAD_NUM_COLS; c++) {
            if (digitalRead(kColPins[c]) == LOW) colMask |= (1U << c);
        }
        digitalWrite(kRowPins[r], HIGH);
        for (uint8_t c = 0; c < KEYPAD_NUM_COLS; c++) {
            if (colMask & (1U << c)) {
                key = kKeyMap[r][c];
                break;
            }
        }
    }
    return key;
}

void KeypadReader::update() {
    const KeyCode now = scan();
    if (now == lastScan_ && now != stableKey_) {
        stableKey_ = now;
        if (now != 0) pendingKey_ = now;
    }
    lastScan_ = now;
}

KeyCode KeypadReader::readKey() {
    KeyCode k = pendingKey_;
    pendingKey_ = 0;
    return k;
}
