/**
 * @file arduino.h
 * @brief Adaptador de mayusculas/minusculas para StateMachineLib.
 *
 * StateMachineLib 1.0.0 incluye "arduino.h" (minusculas). En sistemas
 * de archivos sensibles a mayusculas (Linux, CI) ese archivo no existe
 * en el core AVR, que solo provee Arduino.h. Este adaptador redirige
 * al encabezado real con include_next; en Windows (insensible a
 * mayusculas) tambien resuelve <Arduino.h> sin recursion.
 * Ver docs/03_hallazgos.md, H-09.
 */
#include_next <Arduino.h>
