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

## H-04 — Ausencia de AsyncTaskLib en el material del curso

- **Problema:** la rúbrica exige "tareas asíncronas" pero el material
  solo documenta conceptos RTOS (FreeRTOS, colas, prioridades) y
  super-loop; no hay librería concreta indicada.
- **Solución:** se diseñó `TaskScheduler` cooperativo (millis-based)
  como implementación por defecto, con API compatible para migrar a
  FreeRTOS si el profesor lo exige. Pendiente D-04.

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

## H-07 — Toolchain no disponible en el entorno de desarrollo

- **Problema:** ni PlatformIO (`pio`) ni Doxygen están instalados en
  el equipo actual; no fue posible ejecutar compilación ni generar
  HTML de documentación en esta sesión.
- **Solución:** verificación estructural alternativa (parejas
  `.h`/`.cpp`, balance de llaves, cobertura de docblocks) y comandos
  pendientes para cuando se instale el toolchain:
  - `pio run -e megaatmega2560`
  - `doxygen Doxyfile`
- **Pendiente:** instalar PlatformIO IDE/CLI y Doxygen antes de la
  entrega para validar compilación y generar `docs/html`.
- **Actualización:** PlatformIO ya está instalado y operativo en el
  entorno (`pio run -e megaatmega2560` compila con SUCCESS tras
  resolver H-08). Doxygen sigue pendiente.

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

---

## Optimizaciones propuestas (borrador para el informe)

1. Migrar el scheduler cooperativo a FreeRTOS si el profesor lo
   aprueba, ganando preemption real para `task_fsm` y `task_ui`.
2. Usar la FSM tabular en `PROGMEM` (variante del curso) si la RAM
   se vuelve crítica con muchas transiciones.
3. Filtrado `Average` por canal en sensores para lecturas estables
   en el LCD (librería ya incluida en el curso).
4. Compresión del registro de usuarios en EEPROM (campos fijos,
   sin strings dinámicas) para maximizar el historial persistible.
5. Watchdog hardware del ATmega para recuperación automática ante
   bloqueos del scheduler cooperativo.
