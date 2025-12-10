#include "AGameStateMachine.hpp"
#include <stack>
#include <vector>
#include <iostream>
#include "../../../common/gsm/IGameState.hpp"

namespace gsm {

AGameStateMachine::AGameStateMachine() {}

void AGameStateMachine::changeState(std::shared_ptr<IGameState> newState) {
    if (!_states.empty()) {
        _states.top()->exit();
        _states.pop();
    }
    _states.push(newState);
    _states.top()->enter();
}

void AGameStateMachine::pushState(std::shared_ptr<IGameState> newState) {
    _states.push(newState);
    _states.top()->enter();
}

void AGameStateMachine::popState() {
    if (!_states.empty()) {
        _states.top()->exit();
        _states.pop();
    }
}

void AGameStateMachine::requestStateChange(std::shared_ptr<IGameState> newState) {
    _pendingChangeState = newState;
}

void AGameStateMachine::requestStatePush(std::shared_ptr<IGameState> newState) {
    _pendingPushState = newState;
}

void AGameStateMachine::requestStatePop() {
    _pendingPopState = true;
}

void AGameStateMachine::update(float deltaTime) {
    if (_states.empty()) {
        return;
    }

    auto currentState = _states.top();

    currentState->update(deltaTime);

    if (_pendingChangeState) {
        changeState(_pendingChangeState);
        _pendingChangeState.reset();
    }
    if (_pendingPushState) {
        pushState(_pendingPushState);
        _pendingPushState.reset();
    }
    if (_pendingPopState) {
        popState();
        _pendingPopState = false;
    }
}

}  // namespace gsm
