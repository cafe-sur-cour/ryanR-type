#include "AGameStateMachine.hpp"
#include "../../../common/gsm/IGameState.hpp"

namespace gsm {

AGameStateMachine::AGameStateMachine() {
    this->_states = std::stack<std::shared_ptr<IGameState>>();
}

void AGameStateMachine::changeState(std::shared_ptr<IGameState> newState) {
    if (!_states.empty()) {
        _states.top()->exit();
        _states.pop();
    }
    _states.push(newState);
    _states.top()->enter();
}

void AGameStateMachine::pushState(std::shared_ptr<IGameState> newState) {
    if (!_states.empty()) {
        _states.top()->exit();
    }
    _states.push(newState);
    _states.top()->enter();
}

void AGameStateMachine::popState() {
    if (!_states.empty()) {
        _states.top()->exit();
        _states.pop();
    }
    if (!_states.empty()) {
        _states.top()->enter();
    }
}

void AGameStateMachine::update(float deltaTime) {
    if (!_states.empty()) {
        _states.top()->update(deltaTime);
    }
}

}  // namespace gsm
