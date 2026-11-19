/**
 * @file config.h
 * @brief Configuracion central del sistema: pines, tiempos y
 *        parametros confirmados por el docente.
 *
 * Hardware oficial [CONFIRMADO]: Arduino Mega 2560. Sin RTC, sin
 * cerradura mecanica/servo, sin sensor de fuego y sin HC-SR04.
 * Toda temporizacion usa millis() con resta sin signo (segura ante
 * el desbordamiento de 32 bits). Ver docs/02_pinout.md.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* ================================================================
 *  LCD 16x2 (modo 4 bits)
 * ================================================================ */
#define PIN_LCD_RS   8   ///< Pin RS del LCD 16x2
#define PIN_LCD_EN   9   ///< Pin EN (enable) del LCD 16x2
#define PIN_LCD_D4   10  ///< Pin dato D4 del LCD (modo 4 bits)
#define PIN_LCD_D5   11  ///< Pin dato D5 del LCD (modo 4 bits)
#define PIN_LCD_D6   12  ///< Pin dato D6 del LCD (modo 4 bits)
#define PIN_LCD_D7   13  ///< Pin dato D7 del LCD (modo 4 bits)
#define LCD_COLUMNS  16  ///< Columnas del display LCD
#define LCD_ROWS     2   ///< Filas del display LCD

/* ================================================================
 *  TECLADO MATRICIAL 4x4 (pines 30-37 = PORTC completo del Mega)
 * ================================================================ */
#define KEYPAD_NUM_ROWS 4          ///< Numero de filas del teclado matricial
#define KEYPAD_NUM_COLS 4          ///< Numero de columnas del teclado matricial
#define PIN_KEYPAD_ROWS {30, 31, 32, 33} ///< Pines de las filas del teclado (salidas)
#define PIN_KEYPAD_COLS {34, 35, 36, 37} ///< Pines de las columnas del teclado (entradas pull-up)
#define KEY_ENTER   '#'  ///< Tecla que confirma la contrasena
#define KEY_DELETE  '*'  ///< Tecla que borra el ultimo digito
#define KEY_CANCEL  'D'  ///< Tecla que cancela la autenticacion en curso

/* ================================================================
 *  RFID MFRC522 (SPI hardware del Mega: MOSI=51, MISO=50, SCK=52)
 * ================================================================ */
#define PIN_RFID_SS   53   ///< SDA/SS del RC522
#define PIN_RFID_RST  5    ///< RST del RC522
#define RFID_UID_SIZE_SINGLE 4 ///< Longitud de UID MIFARE Classic (bytes)
#define RFID_UID_SIZE_DOUBLE 7 ///< Longitud de UID MIFARE Ultralight/NTAG (bytes)
#define RFID_UID_MAX_SIZE    7 ///< Bytes reservados por UID
#ifndef WOKWI_SIMULATION
#define WOKWI_SIMULATION     0 ///< 1 en [env:wokwi]: el board-mfrc522 de Wokwi entrega solo 4 bytes del NFC Tag de 7
#endif

/* ================================================================
 *  SENSOR DE EFECTO HALL (estado de puerta)
 * ================================================================ */
#define PIN_HALL_DOOR 40   ///< Salida digital del sensor Hall (LOW = iman presente = puerta cerrada)

/* ================================================================
 *  SENSORES AMBIENTALES
 * ================================================================ */
#define PIN_DHT       22   ///< DHT11 temperatura y humedad
#ifndef DHT_TYPE
#define DHT_TYPE      DHT11 ///< Modelo del sensor DHT (el entorno wokwi usa DHT22)
#endif
#define PIN_LDR       A0   ///< Sensor de luz LDR (divisor resistivo, ADC)
#define PIN_ANALOG_AUX A1  ///< Sensor analogico adicional trabajado en clase (ADC)
#define ANALOG_AVG_SAMPLES 8 ///< Muestras del promedio movil (libreria Average)
#define ADC_VREF_V         5.0f ///< Referencia del ADC (V) para convertir lecturas a voltaje
#define ADC_MAX_COUNT      1023.0f ///< Cuenta maxima del ADC de 10 bits

/* ================================================================
 *  ACTUADORES Y SENALIZACION
 * ================================================================ */
#define PIN_FAN       44   ///< Ventilacion (rele)
#define PIN_LIGHT_OUT 46   ///< Iluminacion (rele/LED)
#define PIN_BUZZER    48   ///< Buzzer de acceso/alarma
#define PIN_LED_RED   6    ///< LED rojo: acceso denegado / bloqueo
#define PIN_LED_GREEN 7    ///< LED verde: acceso permitido
#define RELAY_ACTIVE_LEVEL HIGH ///< Nivel logico que activa los modulos rele (HIGH o LOW).

/* ================================================================
 *  CONTROL DE ACCESO (2FA: RFID + contrasena)
 * ================================================================ */
#define NUM_USERS              3        ///< Usuarios registrados (exactamente 3)
#define MIN_PASSWORD_LENGTH    4        ///< Longitud minima de contrasena
#define MAX_PASSWORD_LENGTH    8        ///< Longitud maxima de contrasena
#define MAX_AUTH_ATTEMPTS      3        ///< Intentos fallidos antes de ALR
#define OPEN_DURATION_MS       5000UL   ///< Tiempo en OPEN (LED verde encendido)
#define GRANTED_BEEP_MS        1000UL   ///< Duracion del tono de acceso permitido
#define ERR_DISPLAY_MS         2000UL   ///< Tiempo del mensaje de error en LCD
#define ERR_BEEP_MS            200UL    ///< Duracion del tono de error
#define LOCKOUT_DURATION_MS    30000UL  ///< Bloqueo temporal en ALR
#define ALARM_BEEP_PERIOD_MS   500UL    ///< Periodo del tono intermitente de alarma
#define PASSWORD_TIMEOUT_MS    15000UL  ///< Tiempo maximo para digitar la contrasena

#define BUZZER_FREQ_OK_HZ      2000U    ///< Frecuencia del tono de acceso permitido
#define BUZZER_FREQ_ERR_HZ     400U     ///< Frecuencia del tono de error/alarma

/* ================================================================
 *  EEPROM
 * ================================================================ */
#define EEPROM_BASE_ADDR       0        ///< Direccion base de la imagen de usuarios
#define EEPROM_MAGIC           0xAC55U  ///< Firma de imagen valida
#define EEPROM_LAYOUT_VERSION  2        ///< Version del esquema de datos persistido (2: UID de 4/7 bytes)

/* ================================================================
 *  CONFORT TERMICO: PMV (Fanger, ISO 7730)
 * ================================================================ */
#define PMV_MET                1.2f     ///< Tasa metabolica (met)
#define PMV_CLO                0.5f     ///< Aislamiento de la ropa (clo)
#define PMV_AIR_SPEED          0.1f     ///< Velocidad relativa del aire (m/s)
#define PMV_EXTERNAL_WORK      0.0f     ///< Trabajo externo (met)
#define PMV_FAN_ON             0.5f     ///< Umbral PMV para encender ventilacion
#define PMV_FAN_OFF            0.2f     ///< Umbral PMV para apagar ventilacion (histeresis)

/* ================================================================
 *  ILUMINACION AUTOMATICA (LDR, ADC 0-1023; mayor = mas luz)
 * ================================================================ */
#ifndef LDR_ADC_RISES_WITH_DARK
#define LDR_ADC_RISES_WITH_DARK 0 ///< 1 si el ADC del LDR sube con la oscuridad (modulo Wokwi); se invierte a mayor = mas luz
#endif
#define LIGHT_ON_BELOW_ADC     300      ///< Enciende iluminacion por debajo de este nivel
#define LIGHT_OFF_ABOVE_ADC    400      ///< Apaga iluminacion por encima de este nivel

/* ================================================================
 *  PERIODOS DE TAREAS (AsyncTaskLib)
 * ================================================================ */
#define TASK_PERIOD_FSM_MS        20UL    ///< Evaluacion de transiciones de la FSM (ms)
#define TASK_PERIOD_KEYPAD_MS     30UL    ///< Escaneo del teclado (ms)
#define TASK_PERIOD_RFID_MS       100UL   ///< Lectura RFID (ms)
#define TASK_PERIOD_DOOR_MS       50UL    ///< Lectura del sensor Hall (ms)
#define TASK_PERIOD_ENV_MS        2000UL  ///< Sensores ambientales + PMV (ms, DHT11 >= 1 s)
#define TASK_PERIOD_ACTUATORS_MS  50UL    ///< Actualizacion de actuadores (ms)
#define TASK_PERIOD_LCD_MS        250UL   ///< Refresco del LCD (ms)
#define TASK_PERIOD_TELEMETRY_MS  2000UL  ///< Telemetria ambiental por Serial (ms)

#endif /* CONFIG_H */
