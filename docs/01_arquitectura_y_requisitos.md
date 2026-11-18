# 01 — Arquitectura y Requisitos
## Sistema de Control de Acceso y Seguridad con Monitoreo de Confort Térmico

- **Asignatura:** Arquitectura Computacional — Universidad del Cauca
- **Docente:** Prof. Fulvio Yesid Vivas Cantero
- **Estado:** Especificación **validada por el docente** (ver `03_hallazgos.md`, H-09). Esqueletos de `src/` implementados y compilando para `megaatmega2560`.
- **Convención de clasificación:** `[CONFIRMADO]` = documento oficial o respuesta del docente · `[ESTÁNDAR DE CLASE]` = material del docente · `[FUERA DE ALCANCE]` = excluido por el docente · `[EXTENSIÓN]` = mejora opcional no exigida.

---

## 1. Fuentes oficiales revisadas

| Documento | Contenido relevante |
|---|---|
| `RÚBRICA DE EVALUACIÓN_ARQUITECTURA COMPUTACIONAL.pdf` | Ponderación oficial de criterios (ver §3) |
| `ARQ_Proyecto.pdf` | Enunciado funcional del proyecto |
| `ARQC_TASK.pdf` | Clase: RTOS, tareas, prioridades, colas, alta cohesión/bajo acoplamiento |
| `ARQC_Clase_10_KEYPAD_LCD.pdf` | Teclado matricial (bitmask de puerto) y LCD16x2 |
| `ARQ_01.pdf` | Sistemas embebidos, Doxygen, GitHub, guías de estilo C |
| `Lab_DIG_TTM_02.pdf` | HC-SR04, DHT11, motor DC, servo, relé, ventilador/bomba |
| `Lab_DIG_TTM_04.pdf` | UART, GPS, Bluetooth HC-05, **FSM (3 variantes)** |
| `FSM1.ino`, `FSM_Tabular.ino`, `FSM_Library.ino` | Ejemplos de FSM del curso |
| `rfid-master.zip` | Librería **MFRC522** (miguelbalboa/rfid) + ejemplos |
| `Average-master.zip` | Librería de promedio móvil (filtrado de sensores) |
| `InformeArquitecturaComputacional.pdf` | **Plantilla del informe final** (secciones y Test Backlog) |

---

## 2. Requisitos funcionales

### 2.1 Control de acceso y seguridad — `[CONFIRMADO: ARQ_Proyecto.pdf + validación docente]`

| ID | Requisito | Decisión confirmada |
|----|-----------|---------------------|
| RF-01 | Autenticación 2FA secuencial | **RFID primero, luego contraseña individual** por teclado 4x4 (4–8 dígitos). No existe acceso solo con clave ni solo con tarjeta. |
| RF-02 | Intentos fallidos | **3 intentos** (tarjeta desconocida o clave errónea) → estado `ALR` + bloqueo temporal con `millis()` (`LOCKOUT_DURATION_MS`). |
| RF-03 | Apertura temporal | Sin cerradura mecánica ni servo: acceso permitido = **LED verde + buzzer** durante `OPEN_DURATION_MS`; retorno automático a `IDLE`. |
| RF-04 | Alarma | **LED rojo + buzzer** intermitente en `ALR`; LED rojo + tono corto en `ERR`. |
| RF-05 | Franjas horarias | `[FUERA DE ALCANCE]` — sin RTC DS3231; no hay control horario. |
| RF-06 | Estado de puerta | **Sensor Hall** (pin 40) con anti-rebote; se muestra en LCD (`ABI`/`CER`). |
| RF-07 | Registro de accesos | `[FUERA DE ALCANCE]` — no se requiere historial de eventos (`EventLog` eliminado). El UID leído se imprime por serial para depuración/alta. |
| RF-08 / RF-09 | Alta de usuarios / copia de llaves | `[EXTENSIÓN]` — el sistema maneja exactamente 3 usuarios fijos; `UserManager::setPassword()` permite cambiar claves. |
| RF-11 | Perfiles | 3 usuarios con perfiles distintos: **Seguridad**, **Operario**, **Gerente**. |
| RF-12 | Rotación de claves | `[EXTENSIÓN]` — no incluida en la especificación confirmada. |
| RF-13 | Persistencia | **EEPROM**: imagen con cabecera (magic `0xAC55`, versión de esquema, longitud) + **CRC-16/CCITT**; si es inválida se restauran los usuarios por defecto. |
| RF-14 | Visualización LCD | Estado FSM (`IDLE`, `PASSWORD`, `OPEN`, `ERR`, `ALR`), dígitos enmascarados con `*`, cuenta regresiva, estado de puerta, T/HR, PMV y ventilación. |

### 2.2 Monitoreo ambiental y confort térmico — `[CONFIRMADO]`

| ID | Requisito | Decisión confirmada |
|----|-----------|---------------------|
| RF-15 | Monitoreo ambiental | **DHT11** (T, HR) + **LDR** (`A0`) + **sensor analógico adicional** de clase (`A1`). Sin sensor de fuego. |
| RF-16 | Confort térmico | **PMV formal (Fanger, ISO 7730)** con `Ta` y `HR` del DHT11 y `M=1.2 met`, `Icl=0.5 clo`, `v=0.1 m/s`, `Tr=Ta`. Sin conteo de personas (sin HC-SR04). |
| RF-17 | Actuadores de confort | **Ventilación** (relé pin 44): ON si `PMV > +0.5`, OFF si `PMV <= +0.2` (histéresis). **Iluminación** (pin 46) por LDR con histéresis (`LIGHT_ON_BELOW_ADC`/`LIGHT_OFF_ABOVE_ADC`). |

### 2.3 Calidad y proceso — `[CONFIRMADO]`

| ID | Requisito |
|----|-----------|
| RF-18 | Tareas **asincrónicas** con `AsyncTaskLib` (una `AsyncTask` por responsabilidad) |
| RF-19 | **FSM modular** con `StateMachineLib` |
| RF-20 | Integración de sensores/actuadores alineada al control lógico |
| RF-21 | Interfaz clara con retroalimentación visual y sonora |
| RF-22 | Documentación **Doxygen** completa (`doxygen Doxyfile` sin warnings) |
| RF-23 | Código limpio, estructurado, **modular y reutilizable** |
| RF-24 | Informe con secciones fijas (ver `InformeArquitecturaComputacional.pdf`) y **Test Backlog (mín. 10)** |

---

## 3. Matriz de Trazabilidad (ponderación oficial de la rúbrica)

| # | Criterio | Peso | Requisitos vinculados | Entregable de diseño |
|---|----------|------|-----------------------|----------------------|
| 1 | Diseño de Tareas asincrónicas | **12%** | RF-18 | 7 `AsyncTask` (§7) |
| 2 | FSM | **8%** | RF-19, RF-14 (estados visibles) | `StateMachineLib`, tabla de transiciones (§6) |
| 3 | Integración Hardware (sensores/actuadores) | **10%** | RF-01, RF-15..17, RF-10 | Pinout confirmado (`02_pinout.md`) |
| 4 | Interfaz de Usuario | **8%** | RF-14, RF-21 | Maqueta LCD 16x2, enmascarado, cuenta regresiva |
| 5 | Resultado en el confort térmico | **10%** | RF-15, RF-16, RF-17 | PMV Fanger/ISO 7730 + histéresis de ventilación (§8) |
| 6 | Sistema de Control de Acceso con RFID | **10%** | RF-01..RF-13 | 2FA RFID + clave, `UserManager`/`NvmStorage` (3 usuarios, EEPROM + CRC) |
| 7 | Documentación con Doxygen | **10%** | RF-22 | Docblocks en todos los `.h/.cpp` |
| 8 | Implementación en Hardware | **8%** | Todo el sistema físico | PCB/protoboard, evidencias fotográficas |
| 9 | Calidad del Código | **8%** | RF-23 | Estructura modular, cohesión/alcoupling |
| 10 | Originalidad técnica | **8%** | — | Elementos diferenciadores a definir `[PROPUESTA]` |
| 11 | Trabajo en equipo | **8%** | RF-24 | Distribución de roles, commits GitHub |
| | **TOTAL** | **100%** | | |

> Corrección respecto a la versión anterior: el acceso ya **no** pesa 18%; el 10% de "Integración Hardware" es transversal a sensores/actuadores, y aparecen "Originalidad" y "Trabajo en equipo" que antes no estaban desglosados.

---

## 4. Requisitos no funcionales y hardware

| ID | Requisito | Estado |
|----|-----------|--------|
| RNF-01 | Microcontrolador | `[CONFIRMADO]` **Arduino Mega 2560** (ATmega2560), entorno PlatformIO `megaatmega2560`. |
| RNF-02 | Recursos limitados | `[CONFIRMADO]` Build actual: RAM ≈ 11.5 % (943 B / 8 KB), Flash ≈ 8.4 %. |
| RNF-03 | Tiempo real | `[CONFIRMADO]` Ningún módulo usa `delay()`; toda espera es no bloqueante. |
| RNF-04 | Concurrencia | `[CONFIRMADO]` **`luisllamasbinaburo/AsyncTaskLib`** (cooperativo). El planificador propio `TaskScheduler` fue eliminado. |
| RNF-05 | Temporización | `[CONFIRMADO]` Sin RTC. Toda temporización de la aplicación usa `millis()` con `uint32_t` y resta segura (`millis() - inicio >= duracion`), válida tras el desbordamiento de 32 bits (~49,7 días). |
| RNF-06 | Sensores y actuadores | `[CONFIRMADO]` RFID MFRC522, keypad 4x4, LCD 16x2 (4 bits), Hall, DHT11, LDR, analógico A1, relé de ventilación, iluminación, buzzer, LED rojo/verde. **Excluidos:** fuego, HC-SR04, RTC DS3231, cerradura mecánica, servo. |
| RNF-07 | Lenguaje/estilo | `[ESTÁNDAR DE CLASE]` C++ (Arduino), Doxygen + GitHub. |

### 4.1 Librerías (`platformio.ini`)

| Librería | Uso |
|----------|-----|
| `miguelbalboa/MFRC522@^1.4.9` | Lector RFID por SPI |
| `adafruit/DHT sensor library@^1.4.6` | DHT11 |
| `arduino-libraries/LiquidCrystal@^1.0.7` | LCD 16x2 modo 4 bits |
| `luisllamasbinaburo/StateMachineLib@^1.0.0` | FSM de acceso |
| `luisllamasbinaburo/AsyncTaskLib@^1.0.0` | Tareas asíncronas |
| `lib/Average` (local) | Promedio móvil de LDR y A1 |
| `EEPROM`, `SPI` (framework) | Persistencia y bus SPI |

---

## 5. Arquitectura de módulos (`src/`)

| Módulo | Responsabilidad |
|--------|-----------------|
| `RfidReader` | Polling no bloqueante del MFRC522, entrega UID de 4 bytes |
| `KeypadReader` | Escaneo matricial 4x4 con anti-rebote por `millis()` |
| `DoorSensor` | Lectura del Hall (pull-up, anti-rebote) |
| `EnvSensors` | DHT11 + LDR + A1 (filtrados con `Average`) → `EnvSnapshot` |
| `ComfortController` | PMV/PPD de Fanger y decisión de ventilación/iluminación con histéresis |
| `Actuators` | LEDs, buzzer (patrones no bloqueantes), relés de ventilación/iluminación |
| `UserManager` | 3 usuarios (UID, perfil, contraseña), búsqueda por UID, validación |
| `NvmStorage` | Imagen EEPROM versionada con CRC-16/CCITT |
| `AuthManager` | Validación secuencial tarjeta → contraseña y contador de fallos |
| `AccessFSM` | FSM 2FA sobre `StateMachineLib` |
| `DisplayUI` | Pantallas del LCD por estado |
| `main.cpp` | Instancia módulos y las 7 `AsyncTask`; `loop()` solo llama `Update()` |

Los módulos se comunican por llamadas explícitas desde las tareas (snapshots e interfaces públicas), sin variables globales compartidas entre módulos.

---

## 6. FSM de acceso 2FA (`AccessFSM` + `StateMachineLib`)

### 6.1 Estados

| Estado | LCD | Significado | Salidas |
|--------|-----|-------------|---------|
| `IDLE` | `IDLE` | Espera tarjeta RFID | Señales de acceso apagadas |
| `PASSWORD` (RFID_OK) | `PASSWORD` | Tarjeta válida; captura de contraseña enmascarada con `*` | — |
| `OPEN` | `OPEN` | Acceso concedido, cuenta regresiva | LED verde + buzzer (tono de acceso) |
| `ERR` | `ERR` | Credencial incorrecta con < 3 fallos | LED rojo + tono corto + mensaje |
| `ALR` | `ALR` | 3 fallos: bloqueo temporal | LED rojo + buzzer intermitente, cuenta regresiva |

### 6.2 Tabla de transiciones (10 transiciones)

| Origen | Condición | Destino | Acción de entrada |
|--------|-----------|---------|-------------------|
| IDLE | Tarjeta registrada | PASSWORD | Limpiar buffer, iniciar timeout de clave |
| IDLE | Tarjeta desconocida, fallos < 3 | ERR | Señal `DENIED` |
| IDLE | Tarjeta desconocida, fallos = 3 | ALR | Señal `ALARM` |
| PASSWORD | `#` con clave correcta del usuario de la tarjeta | OPEN | Reset de intentos, señal `GRANTED` |
| PASSWORD | `#` con clave incorrecta, fallos < 3 | ERR | Señal `DENIED` |
| PASSWORD | `#` con clave incorrecta, fallos = 3 | ALR | Señal `ALARM` |
| PASSWORD | `D` (cancelar) o `PASSWORD_TIMEOUT_MS` | IDLE | — |
| OPEN | `OPEN_DURATION_MS` cumplido | IDLE | Apagar señales |
| ERR | `ERR_DISPLAY_MS` cumplido | IDLE | — |
| ALR | `LOCKOUT_DURATION_MS` cumplido | IDLE | Reset de intentos (al salir) |

Teclas: dígitos `0–9` = clave, `*` = borrar último dígito, `#` = confirmar, `D` = cancelar. Las tarjetas se ignoran fuera de `IDLE` y las teclas fuera de `PASSWORD`, por lo que durante `ALR` no se aceptan credenciales.

---

## 7. Tareas asíncronas (`AsyncTaskLib`)

| Tarea | Responsabilidad | Periodo |
|-------|-----------------|---------|
| `taskRfid` | Leer UID del MFRC522 y entregarlo a la FSM | 100 ms |
| `taskKeypad` | Escanear teclado y entregar teclas a la FSM | 30 ms |
| `taskDoor` | Leer sensor Hall | 50 ms |
| `taskEnv` | Leer DHT11/LDR/A1 y calcular PMV → comandos de confort | 2000 ms (DHT11 ≥ 1 s) |
| `taskActuators` | Actualizar patrones de buzzer/LED y relés | 50 ms |
| `taskLcd` | Refrescar LCD 16x2 | 250 ms |
| `taskFsm` | Evaluar transiciones de `AccessFSM` (`StateMachine::Update()`) | 20 ms |

Todas se crean con `AutoReset = true` y se despachan desde `loop()` con `Update()`. `AsyncTaskLib` mide internamente en `micros()` con resta sin signo (segura ante desbordamiento); la lógica de la aplicación usa `millis()`.

---

## 8. Modelo de confort térmico (PMV — Fanger / ISO 7730)

Entradas: `Ta` y `HR` del DHT11; `Tr = Ta`; `M = 1.2 met` (69.84 W/m²); `W = 0`; `Icl = 0.5 clo` (0.0775 m²K/W); `v = 0.1 m/s`.

1. Presión parcial de vapor: `pa = HR·10·exp(16.6536 − 4030.183/(Ta + 235))` [Pa].
2. Factor de área de vestimenta: `fcl = 1.00 + 1.290·Icl` si `Icl ≤ 0.078 m²K/W`, si no `1.05 + 0.645·Icl`.
3. Temperatura superficial de la ropa `tcl` por iteración de punto fijo, con `hc = max(2.38·|tcl − Ta|^0.25, 12.1·√v)`.
4. Pérdidas: difusión por piel, sudor, respiración latente y seca, radiación y convección.
5. `PMV = (0.303·e^(−0.036·M) + 0.028)·L`, donde `L` es la carga térmica; `PPD = 100 − 95·e^(−0.03353·PMV⁴ − 0.2179·PMV²)`.

Validación numérica contra la tabla de ISO 7730 (Anexo D): `Ta=Tr=22 °C, HR=60 %` → PMV −0.75 (calculado −0.752, PPD 16.9 %); `Ta=Tr=27 °C, HR=60 %` → PMV 0.77 (calculado 0.765, PPD 17.3 %).

**Control de ventilación:** ON si `PMV > +0.5`; OFF si `PMV ≤ +0.2`; entre ambos umbrales mantiene el estado (histéresis). Si la lectura del DHT11 es inválida (`dhtValid = false`) no se cambia el estado.

---

## 9. Test Backlog (≥10 casos)

Formato alineado con la plantilla del informe (`# | PRUEBA | DESCRIPCIÓN | PASÓ | OBSERVACIONES`).

| # | Prueba | Descripción | Criterio de aceptación |
|---|--------|-------------|------------------------|
| 1 | 2FA correcta | Tarjeta registrada + clave correcta + `#` | `OPEN`, LED verde + buzzer, cuenta regresiva, vuelve a `IDLE` |
| 2 | Solo clave | Teclear clave en `IDLE` sin tarjeta | Se ignora; permanece en `IDLE` |
| 3 | Tarjeta desconocida | Acercar tarjeta no registrada | `ERR`, LED rojo + tono, intento +1 |
| 4 | Clave incorrecta | Tarjeta válida + clave errónea | `ERR`, intento +1, vuelve a `IDLE` |
| 5 | Bloqueo | 3 fallos consecutivos | `ALR`, LED rojo + buzzer intermitente, credenciales ignoradas hasta fin del bloqueo |
| 6 | Enmascarado | Digitar clave | LCD muestra solo `*`; `*` borra el último dígito |
| 7 | Clave de otro usuario | Tarjeta del usuario 1 + clave del usuario 2 | Rechazo (`ERR`) |
| 8 | Timeout/cancelar | Esperar `PASSWORD_TIMEOUT_MS` o pulsar `D` | Regresa a `IDLE` |
| 9 | Persistencia | Reiniciar la placa | Usuarios se cargan de EEPROM; CRC inválido restaura valores por defecto |
| 10 | Sensor Hall | Acercar/retirar imán | LCD alterna `CER`/`ABI` |
| 11 | Lectura DHT11 | Variar T/HR | LCD muestra T, HR y PMV coherentes |
| 12 | Ventilación ON | Calentar el sensor hasta `PMV > 0.5` | Relé pin 44 activo |
| 13 | Histéresis | Bajar PMV a valores entre 0.2 y 0.5, luego ≤ 0.2 | Mantiene ON; apaga solo en ≤ 0.2 |
| 14 | Iluminación | Cubrir/iluminar LDR | Pin 46 conmuta con histéresis |
| 15 | Desbordamiento de `millis()` | Revisión de código / simulación de `uint32_t` cerca de `0xFFFFFFFF` | Temporizadores siguen siendo correctos |

---

## 10. Decisiones del docente (resueltas)

| ID | Pregunta | Resolución |
|----|----------|------------|
| D-01 | ¿RFID + clave? | **2FA secuencial obligatoria** (RFID → clave individual) |
| D-02 | ¿Modelo de confort? | **PMV formal Fanger/ISO 7730**, ventilación ON `> +0.5`, OFF `≤ +0.2` |
| D-03 | ¿Conteo de personas? | No se requiere (sin HC-SR04) |
| D-04 | ¿Framework de tareas? | **AsyncTaskLib** + **StateMachineLib** |
| D-05 | ¿Hardware exacto? | **Mega 2560**; sin RTC, sin fuego, sin cerradura mecánica/servo (ver `02_pinout.md`) |
| D-06 | ¿Historial de eventos? | No se requiere (`EventLog` eliminado) |
| D-07 | ¿Qué va en EEPROM? | 3 usuarios: UID, perfil y contraseña, con versión + CRC |
| D-08 | ¿Confort dentro de la FSM? | Módulo independiente (`ComfortController`) ejecutado por `taskEnv` |
