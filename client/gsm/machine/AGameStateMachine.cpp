#include "AGameStateMachine.hpp"
#include "../states/IGameState.hpp"

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

void AGameStateMachine::render() {
    if (!_states.empty()) {
        _states.top()->render();
    }
}

}  // namespace gsm
