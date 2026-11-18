# 02 — Pinout del Sistema
## Arduino Mega 2560 (ATmega2560)

- **Estado:** `[CONFIRMADO]` por el docente (ver `03_hallazgos.md`, H-09). Fuente única en código: `include/config.h`.
- **Excluido del hardware:** RTC DS3231, sensor de fuego, HC-SR04, cerradura mecánica/solenoide y servo.
- **Fuente de pines:** datasheet ATmega2560 + ejemplos del curso (`rfid-master.zip`, `ARQC_Clase_10_KEYPAD_LCD.pdf`).

---

## 1. Mapa de pines

### 1.1 Buses

| Función | Pines Mega | Uso |
|---------|-----------|-----|
| UART0 RX/TX | 0, 1 | **Reservados** — programación USB y monitor serial (9600 baud) |
| SPI SS | 53 | MFRC522 SDA/SS |
| SPI MOSI | 51 | MFRC522 MOSI |
| SPI MISO | 50 | MFRC522 MISO |
| SPI SCK | 52 | MFRC522 SCK |
| I2C SDA/SCL | 20, 21 | **Libres** (sin RTC) |

### 1.2 Control de acceso

| Dispositivo | Señal | Pin Mega | Macro `config.h` | Tipo |
|-------------|-------|----------|------------------|------|
| RFID MFRC522 | SDA (SS) | 53 | `PIN_RFID_SS` | SPI |
| RFID MFRC522 | RST | 5 | `PIN_RFID_RST` | Digital out |
| RFID MFRC522 | MOSI / MISO / SCK | 51 / 50 / 52 | (bus SPI hardware) | SPI |
| RFID MFRC522 | 3.3V / GND | 3.3V / GND | — | Alimentación (IRQ sin conectar) |
| Keypad 4x4 | Filas R1–R4 | 30, 31, 32, 33 | `PIN_KEYPAD_ROWS` | Digital out (escaneo) |
| Keypad 4x4 | Columnas C1–C4 | 34, 35, 36, 37 | `PIN_KEYPAD_COLS` | Digital in, pull-up interno |
| LCD 16x2 (4 bits) | RS | 8 | `PIN_LCD_RS` | Digital out |
| LCD 16x2 (4 bits) | EN | 9 | `PIN_LCD_EN` | Digital out |
| LCD 16x2 (4 bits) | D4–D7 | 10, 11, 12, 13 | `PIN_LCD_D4`…`PIN_LCD_D7` | Digital out |
| LCD 16x2 | RW | GND | — | Solo escritura |
| Sensor Hall (puerta) | DO | 40 | `PIN_HALL_DOOR` | Digital in, pull-up; LOW = imán presente = puerta cerrada |

Mapa del teclado: `1 2 3 A / 4 5 6 B / 7 8 9 C / * 0 # D` — `#` confirma, `*` borra, `D` cancela.

### 1.3 Señalización de acceso (sin cerradura mecánica ni servo)

| Actuador | Pin Mega | Macro | Uso |
|----------|----------|-------|-----|
| LED verde | 7 | `PIN_LED_GREEN` | Acceso permitido (`OPEN`) |
| LED rojo | 6 | `PIN_LED_RED` | Acceso denegado (`ERR`) y bloqueo (`ALR`) |
| Buzzer | 48 | `PIN_BUZZER` | `tone()`: 2 kHz acceso, 400 Hz error/alarma |

### 1.4 Sensores ambientales

| Sensor | Señal | Pin Mega | Macro | Tipo |
|--------|-------|----------|-------|------|
| DHT11 | DATA | 22 | `PIN_DHT` | Digital 1-wire (`DHT.h`), lectura cada 2 s |
| LDR | AO | **A0** | `PIN_LDR` | ADC 10 bits (divisor resistivo), promedio móvil |
| Sensor analógico adicional (clase) | AO | **A1** | `PIN_ANALOG_AUX` | ADC 10 bits, promedio móvil |

### 1.5 Actuadores ambientales

| Actuador | Pin Mega | Macro | Control |
|----------|----------|-------|---------|
| Ventilación (relé) | 44 | `PIN_FAN` | PMV > +0.5 → ON; PMV ≤ +0.2 → OFF |
| Iluminación (relé/LED) | 46 | `PIN_LIGHT_OUT` | LDR < 300 → ON; LDR > 400 → OFF |

Nivel activo de relés: `RELAY_ACTIVE_LEVEL` (por defecto `HIGH`; cambiar a `LOW` si el módulo de relé es activo en bajo).

---

## 2. Consideraciones eléctricas

1. **Alimentación de actuadores:** el Mega no alimenta bobinas de relé ni ventiladores; usar fuente externa dedicada.
2. **Relés:** módulos optoacoplados (bornes NO/COM); el pin `IN` se excita desde el Mega.
3. **Tierra común:** GND de la fuente externa, del Mega y de los sensores unidos.
4. **RC522:** alimentar a **3.3 V**, nunca a 5 V.
5. **DHT11:** pull-up ~4.7 kΩ en DATA si el módulo no lo trae; no leer más rápido de 1 lectura/s.
6. **Hall:** módulos con salida en colector abierto funcionan con el pull-up interno; ubicar el imán en la hoja de la puerta.
7. **LEDs:** resistencia 220–330 Ω. **Buzzer:** pasivo (usa `tone()`); con transistor si supera ~20 mA.
8. **Cableado:** agrupar por dominio (lógica / sensores / potencia) y fotografiar el montaje (≥5 fotos para el informe).

---

## 3. Verificación de colisiones

| Recurso | Pines usados | ¿Conflicto? |
|---------|--------------|-------------|
| SPI (RC522) | 50, 51, 52, 53 | Ninguno — único periférico SPI |
| UART0 | 0, 1 | Reservados, sin uso por periféricos |
| LCD | 8–13 | Ninguno |
| Keypad | 30–37 | Ninguno |
| Digitales varios | 5, 6, 7, 22, 40, 44, 46, 48 | Sin traslape |
| Analógicos | A0, A1 | Sin traslape |
| Timers | `tone()` usa Timer2 | No afecta pines del LCD, relés ni LEDs (sin PWM en ellos) |
