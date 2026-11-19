/**
 * @file main.cpp
 * @brief Punto de entrada: instancia los modulos y planifica una
 *        AsyncTask (AsyncTaskLib) por responsabilidad.
 *
 * | Tarea          | Periodo                  | Responsabilidad                     |
 * |----------------|--------------------------|-------------------------------------|
 * | taskRfid       | TASK_PERIOD_RFID_MS      | Leer tarjetas MFRC522               |
 * | taskKeypad     | TASK_PERIOD_KEYPAD_MS    | Escanear teclado 4x4                |
 * | taskDoor       | TASK_PERIOD_DOOR_MS      | Sensor Hall de puerta               |
 * | taskEnv        | TASK_PERIOD_ENV_MS       | DHT11 + LDR + A1 y calculo PMV      |
 * | taskActuators  | TASK_PERIOD_ACTUATORS_MS | Ventilacion, iluminacion, buzzer    |
 * | taskLcd        | TASK_PERIOD_LCD_MS       | Refresco del LCD 16x2               |
 * | taskFsm        | TASK_PERIOD_FSM_MS       | Transiciones de AccessFSM           |
 * | taskTelemetry  | TASK_PERIOD_TELEMETRY_MS | Linea [ENV] de telemetria por Serial|
 *
 * loop() solo llama Update() de cada tarea: ninguna usa delay().
 */

#include <Arduino.h>
#include <AsyncTaskLib.h>
#include <LiquidCrystal.h>
#include "config.h"
#include "NvmStorage.h"
#include "UserManager.h"
#include "AuthManager.h"
#include "Actuators.h"
#include "AccessFSM.h"
#include "KeypadReader.h"
#include "RfidReader.h"
#include "DoorSensor.h"
#include "EnvSensors.h"
#include "ComfortController.h"
#include "DisplayUI.h"

/* ---------------- Modulos ---------------- */
static NvmStorage        storage(EEPROM_BASE_ADDR);
static UserManager       users(storage);
static AuthManager       auth(users);
static Actuators         actuators;
static AccessFSM         fsm(auth, actuators);
static KeypadReader      keypad;
static RfidReader        rfid(PIN_RFID_SS, PIN_RFID_RST);
static DoorSensor        door(PIN_HALL_DOOR, 50);
static EnvSensors        env;
static ComfortController comfort;
static LiquidCrystal     lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4,
                             PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
static DisplayUI         display(lcd);

/* ---------------- Cuerpos de tarea ---------------- */

/** @brief Tarea RFID: lee tarjetas nuevas y las entrega a la FSM. */
static void runRfid() {
    if (!rfid.poll()) return;
    const RfidUid uid = rfid.lastUid();
    Serial.print(F("UID:"));
    for (uint8_t i = 0; i < uid.size; i++) {
        Serial.print(uid.bytes[i] < 0x10 ? F(" 0") : F(" "));
        Serial.print(uid.bytes[i], HEX);
    }
    Serial.print(F(" ("));
    Serial.print(uid.size);
    Serial.println(F(" bytes)"));
    fsm.onCardDetected(uid);
}

/** @brief Tarea teclado: escaneo con anti-rebote y entrega de teclas a la FSM. */
static void runKeypad() {
    keypad.update();
    const KeyCode key = keypad.readKey();
    if (key != 0) fsm.onKey(key);
}

/** @brief Tarea sensor Hall: actualiza el estado filtrado de la puerta. */
static void runDoor() { door.update(); }

/** @brief Tarea ambiental: muestrea sensores y recalcula PMV/acciones de confort. */
static void runEnv() {
    env.update();
    comfort.evaluate(env.snapshot());
}

/** @brief Tarea actuadores: aplica ordenes de confort y patrones del buzzer. */
static void runActuators() {
    const ComfortAction action = comfort.lastAction();
    if (action.fanOn != actuators.fanOn()) actuators.setFan(action.fanOn);
    if (action.lightOn != actuators.lightOn()) actuators.setLight(action.lightOn);
    actuators.update();
}

/** @brief Tarea LCD: construye el snapshot de UI y refresca la pantalla. */
static void runLcd() {
    UiSnapshot ui;
    ui.state    = fsm.state();
    ui.seconds  = fsm.remainingSeconds();
    ui.digits   = fsm.enteredDigits();
    ui.attempts = fsm.failedAttempts();
    ui.message  = fsm.lastMessage();
    ui.userName = fsm.currentUser() >= 0
                      ? UserManager::profileName(users.profileOf(fsm.currentUser()))
                      : nullptr;
    ui.doorOpen = door.isOpen();
    ui.env      = env.snapshot();
    ui.comfort  = comfort.lastAction();
    display.render(ui);
}

/** @brief Tarea FSM: evalua temporizadores y transiciones de acceso. */
static void runFsm() { fsm.update(); }

/**
 * @brief Tarea telemetria: imprime por Serial una linea compacta con
 *        T, HR, PMV, LDR y A1 (promedio movil de ANALOG_AVG_SAMPLES y voltaje).
 *
 * Ejemplo: `[ENV] T=24.0C H=50% PMV=-0.20 LDR=512 A1=512 (2.50V) Puerta=CER`
 */
static void runTelemetry() {
    const EnvSnapshot s = env.snapshot();
    const ComfortAction c = comfort.lastAction();
    Serial.print(F("[ENV] T="));
    if (s.dhtValid) { Serial.print(s.temperatureC, 1); Serial.print(F("C H=")); Serial.print(s.humidityPct, 0); }
    else            { Serial.print(F("--C H=--")); }
    Serial.print(F("% PMV="));
    if (c.valid) Serial.print(c.pmv, 2);
    else         Serial.print(F("--"));
    Serial.print(F(" LDR="));
    Serial.print(s.lightLevel);
    Serial.print(F(" A1="));
    Serial.print(s.auxLevel);
    Serial.print(F(" ("));
    Serial.print(s.auxLevel * (ADC_VREF_V / ADC_MAX_COUNT), 2);
    Serial.print(F("V) Puerta="));
    Serial.println(door.isOpen() ? F("ABI") : F("CER"));
}

/* ---------------- Tareas AsyncTaskLib (autoReset = periodicas) ---------------- */
static AsyncTask taskRfid(TASK_PERIOD_RFID_MS, true, runRfid);
static AsyncTask taskKeypad(TASK_PERIOD_KEYPAD_MS, true, runKeypad);
static AsyncTask taskDoor(TASK_PERIOD_DOOR_MS, true, runDoor);
static AsyncTask taskEnv(TASK_PERIOD_ENV_MS, true, runEnv);
static AsyncTask taskActuators(TASK_PERIOD_ACTUATORS_MS, true, runActuators);
static AsyncTask taskLcd(TASK_PERIOD_LCD_MS, true, runLcd);
static AsyncTask taskFsm(TASK_PERIOD_FSM_MS, true, runFsm);
static AsyncTask taskTelemetry(TASK_PERIOD_TELEMETRY_MS, true, runTelemetry);

/**
 * @brief Inicializacion: modulos, usuarios desde EEPROM y arranque de tareas.
 */
void setup() {
    Serial.begin(9600);
    actuators.begin();
    display.begin();
    if (!users.begin()) {
        Serial.println(F("EEPROM invalida: usuarios por defecto restaurados"));
    }
    keypad.begin();
    rfid.begin();
    door.begin();
    env.begin();
    fsm.begin();

    taskRfid.Start();
    taskKeypad.Start();
    taskDoor.Start();
    taskEnv.Start();
    taskActuators.Start();
    taskLcd.Start();
    taskFsm.Start();
    taskTelemetry.Start();
}

/**
 * @brief Bucle principal: despacha las tareas asincronas sin bloquear.
 */
void loop() {
    taskRfid.Update();
    taskKeypad.Update();
    taskDoor.Update();
    taskEnv.Update();
    taskActuators.Update();
    taskLcd.Update();
    taskFsm.Update();
    taskTelemetry.Update();
}
