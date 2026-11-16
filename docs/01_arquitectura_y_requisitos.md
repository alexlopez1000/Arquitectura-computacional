# 01 — Arquitectura y Requisitos
## Sistema de Control de Acceso y Seguridad con Monitoreo de Confort Térmico

- **Asignatura:** Arquitectura Computacional — Universidad del Cauca
- **Docente:** Prof. Fulvio Yesid Vivas Cantero
- **Estado:** Fase de análisis y diseño preliminar. **NO hay código fuente definitivo.**
- **Convención de clasificación:** `[CONFIRMADO]` = documento oficial · `[ESTÁNDAR DE CLASE]` = material del docente · `[PENDIENTE DEL PROFESOR]` = ambigüedad abierta · `[PROPUESTA TÉCNICA]` = sugerencia no validada.

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

### 2.1 Control de acceso y seguridad — `[CONFIRMADO: ARQ_Proyecto.pdf]`

| ID | Requisito | Detalle literal del enunciado |
|----|-----------|-------------------------------|
| RF-01 | Clave numérica | Ingreso de clave de **al menos 4 dígitos**; validada contra valor en memoria |
| RF-02 | Intentos fallidos | **Máximo 3 intentos** → alarma + **bloqueo temporal** |
| RF-03 | Cerradura temporal | Desbloqueo **solo por tiempo programado**; retorno automático al inicio |
| RF-04 | Alarma | Sonora y/o visual ante intentos excesivos **o intrusión** |
| RF-05 | Franjas horarias | Establecer horarios de acceso permitidos |
| RF-06 | Estado de puerta | Monitorear estado de la puerta (abierta/cerrada) |
| RF-07 | Registro de accesos | Control y monitorización total de accesos, llaves y dispositivos activos |
| RF-08 | Alta de usuarios | Dar de alta nuevos usuarios y accesos en pocos segundos |
| RF-09 | Copia de llaves | Hacer nuevas copias de llaves cuando se necesite |
| RF-10 | Mecanismo de bloqueo | Cerrojo estándar, pestillo o combinación; activar/desactivar pestillo |
| RF-11 | Perfiles | Roles: **Seguridad, Operarios, Coordinadores, Gerentes** |
| RF-12 | Gestión de claves | Claves cambian **después de 4 usos** y **no pueden repetirse** |
| RF-13 | Persistencia | Datos en memoria no volátil (EEPROM) `[CONFIRMADO en resumen de rúbrica/enunciado; verificar si aplica a usuarios]` |
| RF-14 | Visualización LCD | Estado FSM (`IDLE`, `ERR`, `OPEN`, `ALR`), conteo regresivo, dígitos enmascarados, info ambiental, mensajes |

> Nota: el enunciado oficial menciona "clave numérica" y **no menciona RFID explícitamente** en el texto de `ARQ_Proyecto.pdf`. La rúbrica sí evalúa "Sistema de Control de Acceso **con RFID**" (10%). La integración RFID + teclado matricial viene del resumen del proyecto → clasificar combinación exacta como `[PENDIENTE DEL PROFESOR]` (ver D-01).

### 2.2 Monitoreo ambiental y confort térmico — `[CONFIRMADO: ARQ_Proyecto.pdf]`

| ID | Requisito |
|----|-----------|
| RF-15 | Monitorear **temperatura, humedad y fuego** |
| RF-16 | Mantener **confort apropiado** según **número de personas** (aforo) |
| RF-17 | Activar actuadores de confort en tiempo real (ventilación/iluminación) |

### 2.3 Calidad y proceso — `[CONFIRMADO]`

| ID | Requisito |
|----|-----------|
| RF-18 | Tareas **asincrónicas** bien gestionadas y planificación eficiente |
| RF-19 | **FSM modulares**, bien estructuradas |
| RF-20 | Integración de sensores/actuadores alineada al control lógico |
| RF-21 | Interfaz clara con retroalimentación visual y sonora |
| RF-22 | Documentación **Doxygen**: completa, clara, etiquetas, diagramas, referencias |
| RF-23 | Código limpio, estructurado, **modular y reutilizable** |
| RF-24 | Informe con secciones fijas (ver `InformeArquitecturaComputacional.pdf`): planteamiento, diagrama general, módulos, flujo, FSM, componentes software, GUI, alarmas, **Test Backlog (mín. 10)**, diagrama final, ≥5 fotos, ≥4 problemas, ≥5 optimizaciones, link GitHub |

---

## 3. Matriz de Trazabilidad (ponderación oficial de la rúbrica)

| # | Criterio | Peso | Requisitos vinculados | Entregable de diseño |
|---|----------|------|-----------------------|----------------------|
| 1 | Diseño de Tareas asincrónicas | **12%** | RF-18 | Diagrama de tareas (§6), decisión RTOS vs super-loop (§7) |
| 2 | FSM | **8%** | RF-19, RF-14 (estados visibles) | Tabla de transiciones (§5), implementación tabular/library |
| 3 | Integración Hardware (sensores/actuadores) | **10%** | RF-01, RF-15..17, RF-10 | Interfaces por periférico, pines `[ESTÁNDAR DE CLASE]` |
| 4 | Interfaz de Usuario | **8%** | RF-14, RF-21 | Maqueta LCD 16x2, enmascarado, cuenta regresiva |
| 5 | Resultado en el confort térmico | **10%** | RF-15, RF-16, RF-17 | Algoritmo de confort + actuadores |
| 6 | Sistema de Control de Acceso con RFID | **10%** | RF-01..RF-13 | Módulo `AccessControl`, perfiles, EEPROM |
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
| RNF-01 | Microcontrolador | **`[PENDIENTE DEL PROFESOR]`** — NO especificado en documentos oficiales. Ejemplos de clase usan PIC18F4550, Arduino y ESP32. Arduino Mega 2560 queda como `[PROPUESTA TÉCNICA]` por el contexto del proyecto. El ejemplo RFID sí documenta pines para **Mega** (SPI: SS=53, MOSI=51, MISO=50, SCK=52). |
| RNF-02 | Recursos limitados (memoria, procesamiento, alimentación) | `[CONFIRMADO: ARQ_01]` |
| RNF-03 | Tiempo real: respuesta determinista de alarmas y cerradura | `[CONFIRMADO]` |
| RNF-04 | Framework de concurrencia | **`[PENDIENTE DEL PROFESOR]`** — la clase enseña RTOS (FreeRTOS, RIOT, Zephyr…) y el patrón super-loop. **No se encontró `AsyncTaskLib` en el material.** Propuesta: cooperative scheduler con `millis()` tipo AsyncTask, o FreeRTOS si el profesor lo exige. |
| RNF-05 | RTC para franjas horarias | **`[PENDIENTE DEL PROFESOR]`** — DS3231 es propuesta razonable. |
| RNF-06 | Sensores disponibles | **`[PENDIENTE DEL PROFESOR]`** — el material de clase cubre: DHT11 (T°/HR), HC-SR04 (ultrasónico → aforo/puerta), LDR probable para luz, sensor de llama típico, keypad 4x4, LCD16x2, buzzer, LEDs, relés/servo para cerradura. |
| RNF-07 | Lenguaje/estilo | `[ESTÁNDAR DE CLASE]` C/C++ estilo guía C; Doxygen + GitHub obligatorios. |

---

## 5. Estándares técnicos del curso `[ESTÁNDAR DE CLASE]`

### 5.1 FSM — tres variantes enseñadas en `Lab_DIG_TTM_04.pdf`

| Variante | Patrón | Archivo de referencia |
|----------|--------|------------------------|
| FSM por funciones | `enum State` + `enum Input`, `currentState`/`currentInput` globales, `stateX()` por estado, `changeState()`, `Update()` | `FSM1.ino` |
| FSM tabular | Matriz `stateChange[estado][entrada]` en `PROGMEM` + vector de outputs | `FSM_Tabular.ino` |
| FSM con librería | `StateMachine stateMachine(numStates, numTransitions)`; `AddTransition(in, out, condition)`, `SetOnEntering/SetOnLeaving`, `SetState()`, `Update()` | `FSM_Library.ino` + `StateMachineLib.h` |

**Decisión de diseño `[PROPUESTA TÉCNICA]`:** usar la **variante Library** (`StateMachineLib`) por ser la más modular y alineada con "FSM modulares" de la rúbrica; la tabular es alternativa si se requiere ahorro de RAM. Los 4 estados visibles en LCD (`IDLE/ERR/OPEN/ALR`) serían los **estados principales**; se prevén **sub-estados internos** (§5.2) que no necesariamente se exponen.

### 5.2 Periféricos y librerías con soporte en material del curso

| Subsistema | Librería/Componente | Evidencia |
|------------|--------------------|-----------|
| RFID | **MFRC522** (`SPI.h` + `MFRC522.h`), clase `MFRC522 rfid(SS_PIN, RST_PIN)` | `rfid-master.zip`, `ReadNUID.ino` |
| Teclado | Lectura por **bitmask de puerto** (filas salida/columnas entrada) | `ARQC_Clase_10_KEYPAD_LCD.pdf` |
| LCD | LCD16x2, modo 4 u 8 bits, pines RS/RW/EN + D0–D7 | `ARQC_Clase_10_KEYPAD_LCD.pdf`, `LCD16x2 Datasheet.pdf` |
| T°/HR | **DHT11** vía librería `DHT.h` (`readHumidity()`, `readTemperature()`) | `Lab_DIG_TTM_02.pdf` |
| Distancia/aforo | **HC-SR04** (`pulseIn`, `SOUND_SPEED 0.034`) | `Lab_DIG_TTM_02.pdf` |
| Cerradura/actuadores | **Relé** (`digitalWrite`), **servo** (`Servo.h`, PWM 50 Hz), ventilador/bomba por relé | `Lab_DIG_TTM_02.pdf` |
| Filtrado | Librería `Average` (promedio móvil) | `Average-master.zip` |
| Botones | Patrón `ButtonDebounce` (constructor + `update()` + `state()`) | `InformeArquitecturaComputacional.pdf` |
| Hora | RTC — **sin evidencia en material** `[PENDIENTE]` | — |

### 5.3 Pines RFID en Arduino Mega `[ESTÁNDAR DE CLASE — tabla del ejemplo oficial]`

| Señal MFRC522 | Pin Mega |
|--------------|----------|
| RST | 5 |
| SDA (SS) | 53 |
| MOSI | 51 |
| MISO | 50 |
| SCK | 52 |

---

## 6. Diseño conceptual de la FSM

### 6.1 Estados principales (visibles en LCD)

`IDLE` · `ERR` · `OPEN` · `ALR` — según enunciado.

### 6.2 Sub-estados internos `[PROPUESTA TÉCNICA]`

Para cumplir "modular" sin multiplicar estados visibles:

- `AUTH_KEYPAD` — captura de clave (dígitos enmascarados)
- `AUTH_RFID` — espera de tarjeta
- `LOCKOUT` — bloqueo temporal tras 3 fallos o intrusión
- `UNLOCKED` — cerradura abierta, cuenta regresiva activa
- `DOOR_VIOLATION` — puerta abierta fuera de estado válido
- `ENV_MONITOR` — operación concurrente de confort (¿estado o tarea paralela? `[PENDIENTE: decidir si confort es FSM aparte]`)

### 6.3 Tabla de transiciones (borrador)

| Estado actual | Evento/condición | Estado destino | Acciones |
|---------------|------------------|----------------|----------|
| IDLE | Tecla presionada | AUTH_KEYPAD | LCD muestra `*`, contador de dígitos |
| IDLE | Tarjeta RFID válida detectada | AUTH_RFID→(evalúa) | Leer NUID, validar perfil |
| IDLE | Sensor puerta = abierta inesperada | ALR | Buzzer+LED, registrar evento |
| AUTH_KEYPAD | 4+ dígitos ingresados | evalúa clave | Comparar con EEPROM |
| evalúa clave | Clave correcta + franja horaria válida + perfil permite | OPEN | Desbloquear cerradura, cuenta regresiva |
| evalúa clave | Clave correcta pero fuera de franja | ERR | LCD: "Fuera de horario" |
| evalúa clave | Clave incorrecta (intentos <3) | IDLE/AUTH | `ERR` breve, incrementa contador |
| evalúa clave | 3er intento fallido | ALR + LOCKOUT | Alarma, bloqueo temporal |
| OPEN | Cuenta regresiva llega a 0 | IDLE | Re-bloquear cerradura, resetear intentos |
| ALR | Temporizador de alarma/lockout expira | IDLE | Silenciar, resetear |
| cualquier | Clave con 4 usos cumplidos | (interno) | Forzar cambio de clave `[RF-12]` |
| IDLE/OPEN | Confort: T°/HR/aforo fuera de rango | actuadores on/off | Ventilación/iluminación (¿transición o salida?) `[PENDIENTE]` |

> La activación de confort puede modelarse como **salidas de la FSM** o como **FSM independiente**; recomendado: módulo `ComfortController` separado que la FSM solo consulta, para no acoplar ambos dominios (alineado con alta cohesión de la clase de tareas).

---

## 7. Diagrama de tareas asíncronas `[PROPUESTA TÉCNICA — pendiente aprobación]`

Criterio de la clase (`ARQC_TASK.pdf`): **alta cohesión** (una responsabilidad por tarea), **bajo acoplamiento** (comunicación solo por colas/mecanismos abstractos, nunca variables globales compartidas).

| Tarea | Responsabilidad única | Periodo típico | Prioridad | Notas |
|-------|----------------------|----------------|-----------|-------|
| `task_fsm` | Ejecutar `Update()` de la máquina de estados | 10–20 ms | Alta | Única que muta `currentState` |
| `task_input_keypad` | Escanear teclado matricial, publicar tecla | 20–50 ms | Alta | Anti-rebote por tiempo |
| `task_input_rfid` | Polling MFRC522 no bloqueante | 50–100 ms | Media | `PICC_IsNewCardPresent` es lento → chunking |
| `task_sensors_env` | Leer DHT11, LDR/fuego, ultrasonido | 1–2 s (DHT es lento) | Media | Respetar tiempos del sensor |
| `task_comfort` | Evaluar confort + comandar actuadores | 500 ms–1 s | Media | Lee snapshot de sensores por cola |
| `task_access_log` | Persistir eventos/accesos en EEPROM | Bajo demanda | Baja | Escrituras EEPROM espaciadas (desgaste) |
| `task_ui` | Refrescar LCD, LEDs, buzzer | 100–200 ms | Media | Enmascarado y cuenta regresiva |
| `task_schedule` | Validar franjas horarias vía RTC | 1 s | Baja | Solo lectura RTC |

**Mecanismo de comunicación propuesto:** cola de eventos (`struct Event {type, payload}`) + snapshots atómicos de variables compartidas. Si se elige FreeRTOS: `xQueueCreate` + prioridades reales; si cooperative: `AsyncTask`-style con `millis()` por tarea.

---

## 8. Test Backlog (borrador — ≥10 casos)

Formato alineado con la plantilla del informe (`# | PRUEBA | DESCRIPCIÓN | PASÓ | OBSERVACIONES`).

| # | Prueba | Descripción | Criterio de aceptación |
|---|--------|-------------|------------------------|
| 1 | Clave correcta | Ingresar clave válida por keypad | Cerradura se desbloquea; LCD muestra `OPEN` + cuenta regresiva |
| 2 | Clave incorrecta | Ingresar clave errónea | LCD `ERR`, intento incrementa, sin desbloqueo |
| 3 | 3 intentos fallidos | Fallar 3 veces seguidas | Alarma sonora+visual, `ALR`, bloqueo temporal, retorno a `IDLE` al expirar |
| 4 | Retorno a reposo | Esperar fin de cuenta regresiva en `OPEN` | Cerradura re-bloquea, estado vuelve a `IDLE`, intentos reseteados |
| 5 | Dígitos enmascarados | Teclear clave en LCD | Solo se muestran `*`, nunca el dígito real |
| 6 | RFID válido | Acercar tarjeta registrada | Autenticación exitosa (según D-01), desbloqueo |
| 7 | RFID no registrado | Acercar tarjeta desconocida | Rechazo + registro de intento fallido |
| 8 | Franja horaria | Intentar acceso fuera de horario permitido | Acceso denegado con mensaje en LCD |
| 9 | Intrusión de puerta | Abrir puerta en estado `IDLE`/bloqueada | Alarma + evento registrado |
| 10 | Sensor DHT11 | Variar T°/HR ambiente | LCD muestra valores ambientales correctos |
| 11 | Confort térmico | Simular alta aforo/temperatura | Actuador de ventilación se activa en tiempo real |
| 12 | Cambio de clave | Usar clave 4 veces | Sistema exige nueva clave; clave vieja rechazada; clave repetida rechazada |
| 13 | Persistencia EEPROM | Reiniciar placa tras registrar usuarios | Usuarios/perfiles/contadores de uso se conservan |
| 14 | Perfiles | Validar acceso con cada rol (Seguridad/Operarios/Coordinadores/Gerentes) | Permisos y franjas correctos por perfil |
| 15 | Fuego | Simular detección de fuego | Alarma prioritaria + actuadores de seguridad |
| 16 | Alta de usuario | Registrar nuevo usuario desde keypad/admin | Usuario puede autenticarse de inmediato |
| 17 | Copia de llave | Duplicar credencial de un usuario | Ambas copias autentican; revocación individual funciona |

---

## 9. Decisiones pendientes del profesor (consolidado)

| ID | Pregunta | Impacto |
|----|----------|---------|
| D-01 | ¿RFID + clave = 2FA, independientes, o por perfil? | Diseño de `AuthManager` y FSM |
| D-02 | ¿Fórmula/umbral de confort térmico? (PMV simplificado vs T°/HR/aforo ponderado) | `ComfortController` |
| D-03 | ¿Método de conteo de personas? | Sensor de aforo (¿HC-SR04?) |
| D-04 | Framework: ¿cooperativo (`millis()`/AsyncTask) o FreeRTOS? | Arquitectura de tareas (12% rúbrica) |
| D-05 | Hardware exacto: MCU, RTC DS3231, tipo de cerradura/pestillo, sensores de fuego/luz | Pinout y drivers |
| D-06 | ¿Historial de eventos / GUI en PC requeridos? | Alcance de `EventLog` y comunicación serial |
| D-07 | ¿EEPROM para usuarios/perfiles o solo para claves? | Esquema de `Storage` |
| D-08 | ¿Confort como FSM independiente o salidas de la FSM principal? | Modularidad |

---

## 10. Siguiente entregable

- [ ] Esqueletos de directorios `src/` con `.h/.cpp` por módulo y docblocks Doxygen (`@file`, `@brief`, `@param`, `@return`) — **sin lógica**.
- [ ] Pinout tentativo `[PROPUESTA TÉCNICA]` una vez confirmados sensores.
- [ ] Actualizar este documento cuando llegue respuesta del profesor.
