# 02 — Pinout del Sistema
## Arduino Mega 2560 (ATmega2560)

- **Estado:** PROPUESTA pendiente de confirmación del profesor (D-05).
- **Convención:** `[ESTÁNDAR DE CLASE]` = pin documentado en material del curso · `[PROPUESTA]` = asignación propuesta, sin colisiones de hardware.
- **Fuente de pines:** datasheet ATmega2560 (`atmel-2549-...datasheet.pdf`) + ejemplos del curso.

---

## 1. Mapa de pines propuesto

### 1.1 Comunicación y buses reservados

| Función | Pines Mega | Uso | Estado |
|---------|-----------|-----|--------|
| UART0 TX/RX | 0 (RX0), 1 (TX0) | **RESERVADOS** — bootloader/programación USB-serial | `[CONFIRMADO]` no usar |
| UART1 TX/RX | 18 (TX1), 19 (RX1) | Libre / depuración serial secundaria | `[PROPUESTA]` |
| UART2 TX/RX | 16 (TX2), 17 (RX2) | Libre | `[PROPUESTA]` |
| UART3 TX/RX | 14 (TX3), 15 (RX3) | Libre | `[PROPUESTA]` |
| I2C SDA/SCL | 20 (SDA), 21 (SCL) | RTC DS3231 | `[PROPUESTA - PENDIENTE D-05]` |
| SPI SS | 53 | MFRC522 SDA/SS | `[ESTÁNDAR DE CLASE]` |
| SPI MOSI | 51 | MFRC522 MOSI | `[ESTÁNDAR DE CLASE]` |
| SPI MISO | 50 | MFRC522 MISO | `[ESTÁNDAR DE CLASE]` |
| SPI SCK | 52 | MFRC522 SCK | `[ESTÁNDAR DE CLASE]` |
| PWM extras | 2–13, 44–46 | disponibles (Timer0/1/3/4/5 según pin) | `[PROPUESTA]` |

### 1.2 LCD 16x2 (modo 4 bits) `[ESTÁNDAR DE CLASE: ARQC_Clase_10_KEYPAD_LCD.pdf]`

| Señal LCD | Pin Mega | Tipo |
|-----------|----------|------|
| RS | 8 | Digital out |
| EN | 9 | Digital out |
| D4 | 10 | Digital out |
| D5 | 11 | Digital out |
| D6 | 12 | Digital out |
| D7 | 13 | Digital out |
| RW | GND | atado a tierra (solo escritura) |

> Pines 8–13 soportan PWM (Timer4/Timer5) si se requiere dimming futuro.

### 1.3 Teclado matricial 4x4 `[PROPUESTA - PENDIENTE CONFIRMACIÓN]`

| Señal | Pines Mega | Tipo |
|-------|-----------|------|
| Filas R1–R4 | 30, 31, 32, 33 | Digital out (escaneo por bitmask, patrón clase 10) |
| Columnas C1–C4 | 34, 35, 36, 37 | Digital in con pull-up |

### 1.4 RFID MFRC522 `[ESTÁNDAR DE CLASE: rfid-master.zip — tabla oficial para Mega]`

| Señal RC522 | Pin Mega | Tipo |
|-------------|----------|------|
| SDA (SS) | 53 | SPI |
| MOSI | 51 | SPI |
| MISO | 50 | SPI |
| SCK | 52 | SPI |
| RST | 5 | Digital out |
| 3.3V / GND | 3.3V / GND | Alimentación (ver §2) |
| IRQ | — | No conectado |

### 1.5 Sensores ambientales `[PROPUESTA - PENDIENTE CONFIRMACIÓN]`

| Sensor | Señal | Pin Mega | Tipo |
|--------|-------|----------|------|
| DHT11/DHT22 | DATA | 22 | Digital 1-wire (librería `DHT.h`) `[ESTÁNDAR DE CLASE]` |
| DHT | VCC/GND | 5V / GND | Alimentación |
| Fuego/flama | DO | 24 | Digital in (sensor tipo KY-026 / LM393) |
| Luz (LDR) | AO | **A0** | ADC 10-bit (divisor resistivo) |
| HC-SR04 | TRIG | 28 | Digital out (pulso 10 µs) |
| HC-SR04 | ECHO | 29 | Digital in (`pulseIn`) `[ESTÁNDAR DE CLASE]` |
| Estado de puerta | DO | 40 | Digital in (hall/fin de carrera) |

> D-03 pendiente: el HC-SR04 se propone para conteo de aforo y/o supervisión de puerta; confirmar con el profesor.

### 1.6 Actuadores y alarmas `[PROPUESTA - PENDIENTE CONFIRMACIÓN]`

| Actuador | Pin Mega | Tipo | Notas |
|----------|----------|------|-------|
| Cerradura (solenoide/relé) | 42 | Digital out | D-05: tipo pendiente (solenoide/servo/relé+pestillo) |
| Ventilación (relé) | 44 | Digital out / PWM | Relé módulo optoacoplado `[ESTÁNDAR DE CLASE: Lab_DIG_TTM_02]` |
| Iluminación confort (relé) | 46 | Digital out | Relé o triac según carga |
| Buzzer | 48 | Digital out (PWM) | Tonos por `tone()`/PWM |
| LED rojo (alarma) | 6 | Digital out + resistencia | |
| LED verde (OK) | 7 | Digital out + resistencia | |

---

## 2. Consideraciones eléctricas (criterio Implementación Hardware 8%)

1. **Alimentación de actuadores:** el Mega alimenta lógica (5 V / 3.3 V), pero **NO** las bobinas de relés, solenoides ni motores. Usar fuente externa 5–12 V dedicada según la carga.
2. **Relés:** usar módulos **optoacoplados** con bornes NO/COM para aislar la etapa de potencia; el pin `IN` del módulo se excita desde el pin digital del Mega.
3. **Tierra común (GND):** la tierra de la fuente externa de actuadores, la del Mega y la de los sensores debe ser **común** para referencia de señal; nunca compartir la línea de alimentación de potencia.
4. **RC522:** alimentar a **3.3 V** (no 5 V) y respetar niveles SPI — el módulo ya incluye regulador en la mayoría de placas, verificar jumper.
5. **DHT11:** pull-up ~4.7 kΩ en la línea de datos si el módulo no lo incluye; lectura lenta, espaciada ≥2 s.
6. **HC-SR04:** ECHO entrega 5 V; el Mega tolera 5 V en entradas, pero si se migra a otra placa usar divisor de tensión.
7. **Cerradura:** si es solenoide de 12 V, conmutar con relé; si es servo, alimentar aparte y usar `Servo.h` (PWM 50 Hz, 1–2 ms) `[ESTÁNDAR DE CLASE]`.
8. **Consumibles:** LEDs con resistencia 220–330 Ω; buzzer pasivo con transistor si la corriente supera los ~20 mA del pin.
9. **Cableado:** agrupar por dominio (lógica / sensores / potencia) y documentar con foto del montaje para las evidencias del informe (≥5 fotos requeridas).

---

## 3. Verificación de colisiones

| Recurso | Pines usados | ¿Conflicto? |
|---------|--------------|-------------|
| SPI (RC522) | 50, 51, 52, 53 | Ninguno — único periférico SPI |
| I2C (RTC) | 20, 21 | Ninguno |
| UART0 | 0, 1 | Reservados, sin uso |
| LCD | 8–13 | Sin conflicto (no son SPI/I2C en Mega) |
| Keypad | 30–37 | Sin conflicto |
| Digitales varios | 5–7, 22, 24, 28, 29, 40, 42, 44, 46, 48 | Sin traslape |
| Analógico | A0 (64) | Sin traslape |

**Pendiente de ajuste físico:** si el keypad usa matriz en pines con PWM no hay problema; si el buzzer requiere Timer específico, reubicar a pin PWM libre (2–13, 44–46).

---

## 4. TODOs abiertos sobre este documento

- [ ] Confirmar MCU oficial (D-05) — todo el mapa asume Mega 2560.
- [ ] Confirmar RTC DS3231 y dirección I2C.
- [ ] Confirmar tipo de cerradura/pestillo y si requiere relé adicional.
- [ ] Confirmar método de conteo de personas (HC-SR04 u otro) — D-03.
- [ ] Confirmar si se requiere LCD en modo 8 bits (actual: 4 bits, estándar del curso).
