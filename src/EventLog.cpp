/**
 * @file EventLog.cpp
 * @brief Implementacion del buffer circular de eventos.
 */

#include "EventLog.h"

EventLog::EventLog(uint16_t capacity)
    : buffer_(nullptr), capacity_(capacity), head_(0), size_(0) {
    // TODO: asignar buffer_ (new SystemEvent[capacity]).
}

void EventLog::log(EventType type, uint8_t aux) {
    // TODO: escribir en head_, avanzar circularmente, actualizar size_.
}

SystemEvent EventLog::last() const {
    SystemEvent e = { EventType::AUTH_OK, 0, 0 };
    // TODO: devolver ultimo evento o evento vacio si size_==0.
    return e;
}

uint16_t EventLog::count() const { return size_; }
