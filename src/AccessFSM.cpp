/**
 * @file AccessFSM.cpp
 * @brief Implementacion de la FSM de acceso 2FA con StateMachineLib.
 */

#include "AccessFSM.h"

namespace {
const uint8_t kNumStates = 5;
const uint8_t kNumTransitions = 10;

inline uint8_t idx(FsmState s) { return static_cast<uint8_t>(s); }
} // namespace

AccessFSM* AccessFSM::self_ = nullptr;

AccessFSM::AccessFSM(AuthManager& auth, Actuators& actuators)
    : auth_(auth), actuators_(actuators),
      machine_(kNumStates, kNumTransitions),
      input_(Input::NONE), message_(AccessMessage::NONE), user_(-1),
      password_(), length_(0), stateEnterMs_(0) {
    self_ = this;
}

void AccessFSM::begin() {
    machine_.AddTransition(idx(FsmState::IDLE), idx(FsmState::PASSWORD), condCardOk);
    machine_.AddTransition(idx(FsmState::IDLE), idx(FsmState::ERR), condFail);
    machine_.AddTransition(idx(FsmState::IDLE), idx(FsmState::ALR), condFailLimit);

    machine_.AddTransition(idx(FsmState::PASSWORD), idx(FsmState::OPEN), condPasswordOk);
    machine_.AddTransition(idx(FsmState::PASSWORD), idx(FsmState::ERR), condFail);
    machine_.AddTransition(idx(FsmState::PASSWORD), idx(FsmState::ALR), condFailLimit);
    machine_.AddTransition(idx(FsmState::PASSWORD), idx(FsmState::IDLE), condAbort);

    machine_.AddTransition(idx(FsmState::OPEN), idx(FsmState::IDLE), condTimeout);
    machine_.AddTransition(idx(FsmState::ERR), idx(FsmState::IDLE), condTimeout);
    machine_.AddTransition(idx(FsmState::ALR), idx(FsmState::IDLE), condTimeout);

    machine_.SetOnEntering(idx(FsmState::IDLE), enterIdle);
    machine_.SetOnEntering(idx(FsmState::PASSWORD), enterPassword);
    machine_.SetOnEntering(idx(FsmState::OPEN), enterOpen);
    machine_.SetOnEntering(idx(FsmState::ERR), enterErr);
    machine_.SetOnEntering(idx(FsmState::ALR), enterAlr);
    machine_.SetOnLeaving(idx(FsmState::ALR), leaveAlr);

    machine_.SetState(idx(FsmState::IDLE), false, true);
}

void AccessFSM::onCardDetected(const RfidUid& uid) {
    if (state() != FsmState::IDLE || input_ != Input::NONE) return;
    const int8_t user = auth_.validateCard(uid);
    if (user >= 0) {
        user_ = user;
        input_ = Input::CARD_OK;
    } else {
        registerFailure(AccessMessage::CARD_UNKNOWN);
    }
}

void AccessFSM::onKey(KeyCode key) {
    if (state() != FsmState::PASSWORD || input_ != Input::NONE) return;

    if (key >= '0' && key <= '9') {
        if (length_ < MAX_PASSWORD_LENGTH) {
            password_[length_++] = key;
            password_[length_] = '\0';
        }
    } else if (key == KEY_DELETE) {
        if (length_ > 0) password_[--length_] = '\0';
    } else if (key == KEY_CANCEL) {
        input_ = Input::ABORT;
    } else if (key == KEY_ENTER) {
        if (length_ >= MIN_PASSWORD_LENGTH && auth_.validatePassword(user_, password_)) {
            input_ = Input::PASSWORD_OK;
        } else {
            registerFailure(AccessMessage::WRONG_PASSWORD);
        }
        clearPassword();
    }
}

void AccessFSM::update() {
    if (state() == FsmState::PASSWORD && input_ == Input::NONE && stateTimedOut()) {
        input_ = Input::ABORT;
    }
    machine_.Update();
    input_ = Input::NONE;
}

void AccessFSM::registerFailure(AccessMessage msg) {
    message_ = msg;
    input_ = auth_.registerFailure() ? Input::FAIL_LIMIT : Input::FAIL;
}

FsmState AccessFSM::state() const {
    return static_cast<FsmState>(machine_.GetState());
}

uint8_t AccessFSM::enteredDigits() const { return length_; }

uint32_t AccessFSM::stateDurationMs() const {
    switch (state()) {
        case FsmState::PASSWORD: return PASSWORD_TIMEOUT_MS;
        case FsmState::OPEN:     return OPEN_DURATION_MS;
        case FsmState::ERR:      return ERR_DISPLAY_MS;
        case FsmState::ALR:      return LOCKOUT_DURATION_MS;
        case FsmState::IDLE:
        default:                 return 0;
    }
}

bool AccessFSM::stateTimedOut() const {
    const uint32_t duration = stateDurationMs();
    return duration != 0 && (millis() - stateEnterMs_) >= duration;
}

uint16_t AccessFSM::remainingSeconds() const {
    const uint32_t duration = stateDurationMs();
    const uint32_t elapsed = millis() - stateEnterMs_;
    if (duration == 0 || elapsed >= duration) return 0;
    return static_cast<uint16_t>((duration - elapsed + 999UL) / 1000UL);
}

uint8_t AccessFSM::failedAttempts() const { return auth_.failedAttempts(); }
int8_t AccessFSM::currentUser() const { return user_; }
AccessMessage AccessFSM::lastMessage() const { return message_; }

void AccessFSM::clearPassword() {
    memset(password_, 0, sizeof(password_));
    length_ = 0;
}

void AccessFSM::markEntry() { stateEnterMs_ = millis(); }

/* ---------------- Condiciones (StateMachineLib) ---------------- */

bool AccessFSM::condCardOk()     { return self_->input_ == Input::CARD_OK; }
bool AccessFSM::condPasswordOk() { return self_->input_ == Input::PASSWORD_OK; }
bool AccessFSM::condFail()       { return self_->input_ == Input::FAIL; }
bool AccessFSM::condFailLimit()  { return self_->input_ == Input::FAIL_LIMIT; }
bool AccessFSM::condAbort()      { return self_->input_ == Input::ABORT; }
bool AccessFSM::condTimeout()    { return self_->stateTimedOut(); }

/* ---------------- Acciones de entrada/salida ---------------- */

void AccessFSM::enterIdle() {
    self_->markEntry();
    self_->clearPassword();
    self_->user_ = -1;
    self_->actuators_.setAccessSignal(AccessSignal::NONE);
}

void AccessFSM::enterPassword() {
    self_->markEntry();
    self_->clearPassword();
}

void AccessFSM::enterOpen() {
    self_->markEntry();
    self_->auth_.resetAttempts();
    self_->message_ = AccessMessage::NONE;
    self_->actuators_.setAccessSignal(AccessSignal::GRANTED);
}

void AccessFSM::enterErr() {
    self_->markEntry();
    self_->actuators_.setAccessSignal(AccessSignal::DENIED);
}

void AccessFSM::enterAlr() {
    self_->markEntry();
    self_->actuators_.setAccessSignal(AccessSignal::ALARM);
}

void AccessFSM::leaveAlr() {
    self_->auth_.resetAttempts();
}
