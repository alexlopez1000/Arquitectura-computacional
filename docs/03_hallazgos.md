# 03 — Hallazgos técnicos y notas para el informe
## Insumo para las secciones "Problemas encontrados" y "Optimizaciones propuestas"

Registro de incidencias detectadas durante el diseño. Cada entrada
puede convertirse en un ítem de la sección de problemas del informe
final (mínimo 4 requeridos por la plantilla).

---

## H-01 — Colisión de pines con el bus SPI del MFRC522

- **Problema:** la asignación inicial de pines ubica `PIN_LED_RED=50`
  y `PIN_LED_GREEN=52`, que en el Arduino Mega 2560 corresponden a
  **MISO** y **SCK** del bus SPI. Esto habría impedido la comunicación
  con el lector RFID RC522.
- **Solución:** LEDs reubicados a pines digitales libres 6 y 7;
  verificación cruzada de todos los periféricos contra el mapa SPI
  (50–53), I2C (20/21) y UART0 (0/1) documentada en `02_pinout.md` §3.
- **Lección para el informe:** derivar el pinout desde el mapa de
  periféricos del MCU antes de asignar, no al revés.

## H-02 — Sensor de luz asignado a pin digital en lugar de ADC

- **Problema:** `PIN_LIGHT` estaba en el pin digital 26; un LDR con
  divisor resistivo requiere entrada analógica para lectura por ADC.
- **Solución:** reasignado a `A0` (canal analógico del Mega).

## H-03 — Ambigüedad del microcontrolador oficial

- **Problema:** ningún documento del curso especifica el MCU; los
  ejemplos mezclan PIC18F4550, Arduino genérico y ESP32.
- **Solución:** Mega 2560 asumido como propuesta técnica (hay
  datasheet ATmega2560 en el material y la tabla de pines SPI del
  ejemplo RFID cubre explícitamente el Mega). Documentado como
  decisión pendiente D-05.
- **Actualización:** RESUELTO en H-09 — el docente confirmó Arduino
  Mega 2560.

## H-04 — Ausencia de AsyncTaskLib en el material del curso

- **Problema:** la rúbrica exige "tareas asíncronas" pero el material
  solo documenta conceptos RTOS (FreeRTOS, colas, prioridades) y
  super-loop; no hay librería concreta indicada.
- **Solución:** se diseñó `TaskScheduler` cooperativo (millis-based)
  como implementación por defecto, con API compatible para migrar a
  FreeRTOS si el profesor lo exige. Pendiente D-04.
- **Actualización:** RESUELTO en H-09 — el docente fijó
  `AsyncTaskLib` + `StateMachineLib`; `TaskScheduler` fue eliminado.

## H-05 — DHT11 como cuello de botella temporal

- **Problema:** el sensor de temperatura/humedad del curso (DHT11)
  tarda ~2 s por lectura; si se lee en el loop principal bloquearía
  el teclado y la FSM.
- **Solución:** lectura asignada a tarea periódica dedicada
  (`task_sensors_env`, periodo 2 s) para no bloquear el resto del
  sistema. Documentado en `EnvSensors::update()`.

## H-06 — EEPROM de solo 4 KB en ATmega2560

- **Problema:** persistir usuarios + historial de eventos puede
  exceder la EEPROM si el historial crece.
- **Solución:** `EventLog` es buffer circular en RAM; EEPROM reservada
  solo para configuración/usuarios con estructura versionada + CRC
  (pendiente D-07). Candidato a "optimización propuesta".
- **Actualización:** RESUELTO en H-09 — no se requiere historial;
  `EventLog` fue eliminado y la EEPROM guarda solo los 3 usuarios.

## H-07 — Toolchain no disponible en el entorno de desarrollo

- **Problema:** ni PlatformIO (`pio`) ni Doxygen están instalados en
  el equipo actual; no fue posible ejecutar compilación ni generar
  HTML de documentación en esta sesión.
- **Solución:** verificación estructural alternativa (parejas
  `.h`/`.cpp`, balance de llaves, cobertura de docblocks) y comandos
  pendientes para cuando se instale el toolchain:
  - `pio run -e megaatmega2560`
  - `doxygen Doxyfile`
- **Actualización:** RESUELTO. PlatformIO y Doxygen ya están
  instalados y operativos en el equipo:
  - `pio run -e megaatmega2560` → SUCCESS (tras resolver H-08).
  - `doxygen Doxyfile` → ejecutado con **0 warnings**; generó
    `docs/html/` sin errores. Se agregaron comentarios `///<` a las
    macros de `include/config.h` y docblocks `@brief` a
    `setup()`/`loop()` en `src/main.cpp` para eliminar las últimas
    advertencias.

## H-08 — Resolución de dependencias: PlatformIO vs Arduino IDE

- **Problema:** en Arduino IDE las librerías del curso
  (`LiquidCrystal`, `Average`) vienen preinstaladas o se instalan a
  mano desde el gestor de librerías. En PlatformIO cada dependencia
  debe declararse explícitamente; la compilación real
  (`pio run -e megaatmega2560`) falló con
  `fatal error: LiquidCrystal.h` y `fatal error: Average.h` porque
  ninguna estaba declarada en `lib_deps`.
- **Solución:**
  - `LiquidCrystal` → declarada como
    `arduino-libraries/LiquidCrystal@^1.0.7` en `platformio.ini`
    (paquete oficial equivalente a la librería estándar de Arduino
    IDE).
  - `Average` → la versión exacta del curso no está en el registro
    de PlatformIO, así que `Average-master.zip` se extrajo a
    `lib/Average/` (convención de librería local de PlatformIO).
    `lib/` NO está en `.gitignore`, por lo que queda versionada en
    Git; solo se excluyeron `examples/` y el `.gitignore` propio del
    paquete.
  - `Keypad` (chris--a) → verificado que NO se necesita: el proyecto
    no incluye `<Keypad.h>`; el escaneo matricial se implementa de
    forma propia en `KeypadReader` según el patrón de bitmask de
    puerto de la clase 10.
- **Resultado:** `pio run -e megaatmega2560` → SUCCESS
  (Flash 0.7%, RAM 2.3%).
- **Lección para el informe:** PlatformIO exige declarar cada
  dependencia en `platformio.ini` (`lib_deps`) o proveerla como
  librería local en `lib/`; las librerías "incluidas" de Arduino IDE
  no existen por defecto. Alternativa documentable: declarar
  `lib_deps = ... @ file://Average-master.zip` si se prefiere no
  versionar `lib/`.

## H-09 — Cambios tras la validación con el docente y modelo PMV

- **Contexto:** el docente respondió las preguntas D-01…D-08. La
  arquitectura propuesta (RTC, sensor de fuego, HC-SR04, cerradura,
  historial y planificador propio) no coincidía con las
  especificaciones oficiales.
- **Cambios de hardware** (`include/config.h`, `02_pinout.md`):
  - MCU confirmado: **Arduino Mega 2560**.
  - Acceso: MFRC522 (SS=53, MOSI=51, MISO=50, SCK=52, RST=5),
    keypad 4x4 (30–37), LCD 16x2 en 4 bits (8–13), Hall (40).
  - Sin cerradura mecánica ni servo: acceso permitido = LED verde (7)
    + buzzer (48); denegado/bloqueo = LED rojo (6) + buzzer.
  - Ambiente: DHT11 (22), LDR (A0), sensor analógico de clase (A1);
    ventilación (44) e iluminación (46).
  - **Eliminados:** RTC DS3231, sensor de fuego, HC-SR04, pin de
    cerradura (42).
- **Cambios de software** (`src/`):
  - Eliminados `EventLog`, `ScheduleValidator` y `TaskScheduler`.
  - 7 `AsyncTask` (`AsyncTaskLib`): RFID, teclado, Hall, ambiente +
    PMV, actuadores, LCD y FSM. `loop()` solo llama `Update()`.
  - `AccessFSM` sobre `StateMachineLib` con 5 estados
    (`IDLE → PASSWORD → OPEN`, `ERR`, `ALR`) y 10 transiciones;
    2FA secuencial: la contraseña solo se pide tras una tarjeta
    válida y se valida contra el usuario de esa tarjeta.
  - `UserManager` + `NvmStorage`: exactamente 3 usuarios (UID de 4
    bytes, perfil Seguridad/Operario/Gerente, clave individual) en
    una imagen EEPROM con magic `0xAC55`, versión de esquema y
    CRC-16/CCITT (polinomio `0x1021`). Si la imagen es inválida se
    restauran y guardan los valores por defecto.
  - Nuevo módulo `DoorSensor` (Hall con anti-rebote).
  - Toda temporización de la aplicación usa `uint32_t` y la forma
    `millis() - inicio >= duracion`, correcta incluso cuando
    `millis()` desborda (cada ~49,7 días). No hay `delay()`.
- **Modelo PMV** (`ComfortController`): ecuación de Fanger según
  ISO 7730 con `Ta` y `HR` del DHT11, `Tr = Ta`, `M = 1.2 met`,
  `Icl = 0.5 clo`, `v = 0.1 m/s`, `W = 0`:
  - `pa = HR·10·exp(16.6536 − 4030.183/(Ta + 235))`.
  - `tcl` se obtiene por iteración de punto fijo con
    `hc = max(2.38·|tcl − Ta|^0.25, 12.1·√v)`.
  - `PMV = (0.303·e^(−0.036·M) + 0.028)·(M − W − pérdidas)`, con
    pérdidas por difusión, sudor, respiración, radiación y
    convección; `PPD = 100 − 95·e^(−0.03353·PMV⁴ − 0.2179·PMV²)`.
  - Validación contra el Anexo D de ISO 7730: 22 °C / 60 % →
    −0.752 (norma −0.75); 27 °C / 60 % → 0.765 (norma 0.77).
  - Ventilación: ON si `PMV > +0.5`, OFF si `PMV ≤ +0.2`; entre
    ambos umbrales mantiene el estado (histéresis, evita conmutación
    rápida del relé).
- **Incidencia de compilación:** `StateMachineLib` 1.0.0 incluye
  `"arduino.h"` en minúsculas. En sistemas de archivos sensibles a
  mayúsculas (Linux/CI) el core AVR solo provee `Arduino.h` y la
  compilación falla con `fatal error: arduino.h: No such file or
  directory`.
  - **Solución:** adaptador `include/arduino.h` que hace
    `#include_next <Arduino.h>`, y `build_flags = -I include` en
    `platformio.ini` para que también lo vean las librerías. Funciona
    igual en Windows (insensible a mayúsculas) sin recursión. No se
    modifica la librería descargada en `.pio/`.
- **Nota:** `AsyncTaskLib` mide los periodos internamente con
  `micros()` y resta sin signo, también segura ante desbordamiento.
- **Verificación:** `pio run -e megaatmega2560` → SUCCESS, 0 warnings
  (RAM 11.5 %, Flash 8.4 %); `doxygen Doxyfile` → 0 warnings.

## H-10 — Revisión previa al montaje físico

- **LED rojo en `ERR`:** `Actuators::setAccessSignal()` solo
  encendía `PIN_LED_RED` con `AccessSignal::ALARM`, así que un acceso
  denegado (`ERR`, señal `DENIED`) solo sonaba el buzzer, contra la
  especificación (denegado = LED rojo + buzzer).
  - **Solución:** el LED rojo se enciende con `DENIED` o `ALARM`.
- **Polaridad de relés:** `RELAY_ACTIVE_LEVEL` (`include/config.h`,
  por defecto `HIGH`) define el nivel que activa los módulos relé.
  `setFan()` y `setLight()` derivan de ella los niveles de encendido
  y apagado, así que un módulo activo en bajo solo requiere cambiar
  la macro a `LOW`.
- **Supuestos del modelo PMV (ISO 7730):** el DHT11 solo mide `Ta` y
  `HR`; el resto de variables de Fanger se fijan:
  - `Tr = Ta`: sin sensor de globo; se asume recinto interior sin
    superficies calientes/frías ni radiación solar directa.
  - `v = 0.1 m/s`: aire casi quieto, típico de oficina sin
    ventilador encendido. Con la ventilación activa la velocidad real
    sube, por lo que el PMV calculado sobreestima el calor; la
    histéresis (OFF en `PMV ≤ +0.2`) limita ese efecto.
  - `M = 1.2 met`: actividad sedentaria/de pie ligera.
  - `Icl = 0.5 clo`: ropa ligera de verano.
  - `W = 0`: sin trabajo mecánico externo.
  - ISO 7730 recomienda el PMV para `10–30 °C` y `HR 30–70 %`; fuera
    de ese rango el valor es indicativo. La precisión del DHT11
    (±2 °C, ±5 % HR) se traslada al PMV.
- **Verificación:** `pio run -e megaatmega2560` y `doxygen Doxyfile`
  con 0 errores y 0 warnings.

## H-11 — UID RFID de 4/7 bytes y simulación virtual en Wokwi

- **UID de 4 o 7 bytes:** `RfidUid` (`src/RfidUid.h`) guarda
  `bytes[RFID_UID_MAX_SIZE]` (7) y `size`. Solo se aceptan longitudes
  4 (MIFARE Classic) o 7 (Ultralight/NTAG). `RfidReader::poll()`
  descarta otros tamaños, y los bytes sobrantes valen `0x00`.
  `rfidUidEquals()` compara longitud y los 7 bytes con relleno
  `0x00`. `main.cpp` imprime por Serial solo los `uid.size` bytes
  leídos, más la longitud.
- **EEPROM:** `UserRecord` guarda el `RfidUid` completo (UID + longitud).
  `EEPROM_LAYOUT_VERSION` pasa de 1 a 2, así que una imagen con el
  layout de 4 bytes no se acepta y se reescriben los usuarios por
  defecto.
- **Usuarios por defecto** (coinciden con tarjetas predefinidas del
  lector de Wokwi):

  | Tarjeta Wokwi | UID | Perfil | Clave |
  |---|---|---|---|
  | Blue Card | `01:02:03:04` | Seguridad | `1234` |
  | Green Card | `11:22:33:44` | Operario | `5678` |
  | NFC Tag (7 bytes) | `04:11:22:33:44:55:66` | Gerente | `2468` |

  Red y Yellow Card no están registradas (sirven para probar `ERR`/`ALR`).
  En el laboratorio, cambiar los UID por los de las tarjetas reales e
  incrementar `EEPROM_LAYOUT_VERSION`.
- **Entorno `[env:wokwi]`** (`platformio.ini`): hereda de
  `megaatmega2560` y agrega dos `-D`:
  - `DHT_TYPE=DHT22`: Wokwi solo simula DHT22, y su trama decodificada
    como DHT11 da valores erróneos (p. ej. 24 °C / 40 % → −1 °C / 15 %).
  - `LDR_ADC_RISES_WITH_DARK=1`: el módulo LDR de Wokwi da mayor ADC
    con más oscuridad; el firmware lo invierte (`1023 − ADC`) para
    mantener la convención mayor = más luz.
  El firmware del hardware real sigue siendo `pio run -e megaatmega2560`.
- **`wokwi.toml`:** apunta a `.pio/build/wokwi/firmware.hex` y
  `firmware.elf`. Compilar con `pio run -e wokwi` y abrir con la
  extensión Wokwi de VS Code (o `wokwi-cli`).
- **`diagram.json`** (cableado según `docs/02_pinout.md`):

  | Componente Wokwi | Conexión al Mega |
  |---|---|
  | `board-mfrc522` | SDA(SS)=53, SCK=52, MOSI=51, MISO=50, RST=5, 3.3V, GND |
  | `wokwi-lcd1602` (4 bits) | RS=8, E=9, D4–D7=10–13, RW/V0/K=GND, VDD=5V, A=5V vía 220 Ω |
  | `wokwi-membrane-keypad` | R1–R4=30–33, C1–C4=34–37 |
  | `wokwi-dht22` | SDA=22 |
  | `wokwi-photoresistor-sensor` | AO=A0 |
  | `wokwi-potentiometer` (sensor auxiliar) | SIG=A1 |
  | `wokwi-led` verde / rojo + 220 Ω | 7 / 6 |
  | `wokwi-buzzer` | 48 |
  | `wokwi-slide-switch` (Hall) | común=40, lado 1=GND (LOW = puerta cerrada) |
  | `wokwi-led` azul / amarillo + 220 Ω | 44 ventilación / 46 iluminación |

  El lector se llama `board-mfrc522` en Wokwi (no `wokwi-mfrc522`).
  Los LED 44/46 representan los relés con `RELAY_ACTIVE_LEVEL = HIGH`.
- **Verificación:** `pio run -e megaatmega2560`, `pio run -e wokwi` y
  `doxygen Doxyfile` con 0 errores y 0 warnings.

---

## Optimizaciones propuestas (borrador para el informe)

1. Evaluar FreeRTOS en el futuro si se requiere preemption real;
   hoy se usa `AsyncTaskLib` por decisión del docente.
2. Usar la FSM tabular en `PROGMEM` (variante del curso) si la RAM
   se vuelve crítica con muchas transiciones.
3. Filtrado `Average` por canal en sensores para lecturas estables
   en el LCD (librería ya incluida en el curso).
4. Distribuir escrituras de EEPROM (wear leveling) si se habilita
   el cambio frecuente de contraseñas.
5. Watchdog hardware del ATmega para recuperación automática ante
   bloqueos del scheduler cooperativo.
