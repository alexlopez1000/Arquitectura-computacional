/**
 * @file config.h
 * @brief Configuracion central del sistema: pines, constantes y
 *        parametros pendientes de confirmacion del profesor.
 *
 * Todos los valores marcados con TODO [PENDIENTE D-0X] corresponden
 * a decisiones abiertas listadas en docs/01_arquitectura_y_requisitos.md.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

/* ================================================================
 *  MICROCONTROLADOR
 *  TODO [PENDIENTE D-05]: MCU oficial no confirmado.
 *  Los pines siguientes asumen Arduino Mega 2560 (PROPUESTA).
 * ================================================================ */

/* ================================================================
 *  LCD 16x2  [ESTANDAR DE CLASE: ARQC_Clase_10_KEYPAD_LCD.pdf]
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
 *  TECLADO MATRICIAL 4x4
 *  TODO [PENDIENTE D-05]: pinout pendiente de conexion real.
 * ================================================================ */
#define KEYPAD_NUM_ROWS 4          ///< Numero de filas del teclado matricial
#define KEYPAD_NUM_COLS 4          ///< Numero de columnas del teclado matricial
#define PIN_KEYPAD_ROWS {30, 31, 32, 33} ///< Pines de las filas del teclado (salidas)
#define PIN_KEYPAD_COLS {34, 35, 36, 37} ///< Pines de las columnas del teclado (entradas)

/* ================================================================
 *  RFID MFRC522  [ESTANDAR DE CLASE: rfid-master.zip]
 *  Pines SPI documentados para Arduino Mega en el ejemplo oficial.
 * ================================================================ */
#define PIN_RFID_SS   53   ///< SDA/SS del RC522
#define PIN_RFID_RST  5    ///< RST del RC522
// MOSI=51, MISO=50, SCK=52 (SPI hardware del Mega)

/* ================================================================
 *  SENSOR AMBIENTALES  [PENDIENTE D-05 inventario HW]
 * ================================================================ */
#define PIN_DHT       22   ///< DHT11/DHT22 temperatura y humedad [ESTANDAR DE CLASE]
#define PIN_FIRE      24   ///< Sensor de llama/fuego (tipo y pin pendientes)
#define PIN_LIGHT     A0   ///< Sensor de iluminacion (LDR/fotodiodo, ADC) [PROPUESTA]
#define PIN_TRIGGER   28   ///< HC-SR04 TRIG (aforo/puerta) [ESTANDAR DE CLASE]
#define PIN_ECHO      29   ///< HC-SR04 ECHO
#define PIN_DOOR      40   ///< Fin de carrera / hall de estado de puerta

/* ================================================================
 *  ACTUADORES Y ALARMAS
 * ================================================================ */
#define PIN_LOCK      42   ///< Cerradura: relé/servo/pestillo [PENDIENTE D-05]
#define PIN_FAN       44   ///< Ventilacion (relé) [ESTANDAR DE CLASE]
#define PIN_LIGHT_OUT 46   ///< Iluminacion de confort (relé/triac)
#define PIN_BUZZER    48   ///< Buzzer alarma
#define PIN_LED_RED   6    ///< LED alarma visual
#define PIN_LED_GREEN 7    ///< LED estado OK

/* ================================================================
 *  RTC  [PENDIENTE D-05: DS3231 propuesto, modulo I2C Wire]
 * ================================================================ */
// SDA/SCL por bus I2C hardware del Mega (pins 20/21)

/* ================================================================
 *  PARAMETROS DEL SISTEMA
 * ================================================================ */

/** Duracion del desbloqueo de cerradura tras autenticacion valida (ms). */
#define UNLOCK_DURATION_MS        5000UL

/** Duracion del bloqueo temporal tras 3 intentos fallidos o intrusion (ms). */
#define LOCKOUT_DURATION_MS       30000UL

/** Duracion de la alarma sonora/visual (ms). */
#define ALARM_DURATION_MS         10000UL

/** Maximo de intentos fallidos antes de alarma + bloqueo [CONFIRMADO]. */
#define MAX_AUTH_ATTEMPTS         3

/** Longitud minima de la clave numerica [CONFIRMADO: al menos 4 digitos]. */
#define MIN_KEY_LENGTH            4
#define MAX_KEY_LENGTH            8   ///< Longitud maxima de la clave numerica

/** Usos de una clave antes de exigir cambio [CONFIRMADO: 4 usos]. */
#define KEY_MAX_USES              4

/** Numero maximo de usuarios/credenciales persistidos [PROPUESTA]. */
#define MAX_USERS                 16

/* TODO [PENDIENTE D-04]: periodo de cada tarea asincrona.
 * Valores iniciales propuestos; ajustar al framework elegido. */
#define TASK_PERIOD_FSM_MS        20UL    ///< Periodo de la tarea de la FSM (ms)
#define TASK_PERIOD_KEYPAD_MS     30UL    ///< Periodo del escaneo de teclado (ms)
#define TASK_PERIOD_RFID_MS       100UL   ///< Periodo de lectura RFID (ms)
#define TASK_PERIOD_SENSORS_MS    2000UL  ///< Periodo de lectura de sensores ambientales (ms)
#define TASK_PERIOD_COMFORT_MS    1000UL  ///< Periodo del controlador de confort (ms)
#define TASK_PERIOD_UI_MS         200UL   ///< Periodo de actualizacion del display (ms)
#define TASK_PERIOD_STORAGE_MS    5000UL  ///< Periodo de persistencia EEPROM (ms)
#define TASK_PERIOD_SCHEDULE_MS   1000UL  ///< Periodo de validacion de horario (ms)

/* TODO [PENDIENTE D-03]: metodo de conteo de personas.
 * Si se usa HC-SR04: distancia umbral para contar persona. */
#define PEOPLE_COUNT_THRESHOLD_CM 150.0f ///< Distancia umbral HC-SR04 para contar persona (cm)

/* TODO [PENDIENTE D-02]: umbrales de confort termico.
 * Propuesta inicial: rangos configurables hasta confirmar formula. */
#define COMFORT_TEMP_MIN_C        18.0f   ///< Temperatura minima de confort (C)
#define COMFORT_TEMP_MAX_C        26.0f   ///< Temperatura maxima de confort (C)
#define COMFORT_HUM_MIN_PCT       40.0f   ///< Humedad minima de confort (%)
#define COMFORT_HUM_MAX_PCT       60.0f   ///< Humedad maxima de confort (%)
#define COMFORT_PEOPLE_MAX        8       ///< Aforo maximo de personas

#endif /* CONFIG_H */
