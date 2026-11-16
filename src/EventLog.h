/**
 * @file EventLog.h
 * @brief Registro circular de eventos del sistema.
 *
 * Cubre [RF-07] "control y monitorizacion total de los accesos".
 * Almacena eventos en RAM (buffer circular); la persistencia
 * opcional en EEPROM se delega a NvmStorage.
 *
 * TODO [PENDIENTE D-06]: confirmar si se requiere historial
 * persistente o interfaz PC (Serial/USB).
 */

#ifndef EVENT_LOG_H
#define EVENT_LOG_H

#include <Arduino.h>

/** Tipos de evento registrables. */
enum class EventType : uint8_t {
    AUTH_OK = 0,        ///< Autenticacion exitosa.
    AUTH_FAIL,          ///< Autenticacion fallida.
    LOCKOUT,            ///< Bloqueo temporal activado.
    DOOR_OPEN,          ///< Puerta abierta.
    DOOR_VIOLATION,     ///< Intrusion detectada.
    FIRE,               ///< Fuego detectado.
    KEY_ROTATED,        ///< Clave rotada por usos.
    USER_ADDED,         ///< Usuario registrado.
    COMFORT_ACTION      ///< Accion de confort ejecutada.
};

/** Evento del sistema. */
struct SystemEvent {
    EventType     type;      ///< Tipo de evento.
    unsigned long timestamp; ///< Marca millis() del evento.
    uint8_t       aux;       ///< Dato auxiliar (intento, perfil, etc.).
};

/**
 * @class EventLog
 * @brief Buffer circular en RAM de eventos recientes.
 */
class EventLog {
public:
    /**
     * @brief Constructor.
     * @param capacity Numero maximo de eventos en el buffer.
     */
    explicit EventLog(uint16_t capacity);

    /**
     * @brief Registra un evento en el buffer.
     * @param type Tipo de evento.
     * @param aux Dato auxiliar.
     */
    void log(EventType type, uint8_t aux = 0);

    /**
     * @brief Evento mas reciente.
     * @return Copia del ultimo evento registrado.
     */
    SystemEvent last() const;

    /**
     * @brief Numero de eventos almacenados.
     * @return Cantidad actual.
     */
    uint16_t count() const;

private:
    SystemEvent* buffer_; ///< Almacen circular.
    uint16_t     capacity_; ///< Capacidad maxima.
    uint16_t     head_;     ///< Indice de escritura.
    uint16_t     size_;     ///< Eventos ocupados.
};

#endif /* EVENT_LOG_H */
