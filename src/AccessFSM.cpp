/**
 * @file AccessFSM.cpp
 * @brief Implementacion de la maquina de estados del acceso.
 */

#include "AccessFSM.h"
#include "config.h"

AccessFSM::AccessFSM()
    : state_(FsmState::IDLE), authPhase_(AuthPhase::NONE),
      failedAttempts_(0), stateEnterMs_(0), countdownEndMs_(0) {}

void AccessFSM::begin() {
    // TODO [PENDIENTE D-08]: inicializar StateMachineLib o tabla.
}

void AccessFSM::dispatch(FsmEvent event) {
    // TODO: tabla de transiciones segun docs/01_arquitectura_y_requisitos.md
}

void AccessFSM::update() {
    // TODO: temporizadores de OPEN/ALR/LOCKOUT via millis().
}

FsmState AccessFSM::getState() const { return state_; }
AuthPhase AccessFSM::getAuthPhase() const { return authPhase_; }

uint16_t AccessFSM::getCountdownSeconds() const {
    // TODO: calcular segundos restantes hasta countdownEndMs_.
    return 0;
}

void AccessFSM::onEnterIdle()    { /* TODO: acciones de entrada */ }
void AccessFSM::onEnterErr()     { /* TODO */ }
void AccessFSM::onEnterOpen()    { /* TODO: desbloquear cerradura */ }
void AccessFSM::onEnterAlr()     { /* TODO: activar alarma */ }
void AccessFSM::onEnterKeyEntry(){ /* TODO */ }
void AccessFSM::onEnterRfidWait(){ /* TODO */ }
void AccessFSM::onEnterLockout() { /* TODO */ }
