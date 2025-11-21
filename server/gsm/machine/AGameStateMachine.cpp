#include "AGameStateMachine.hpp"
#include "../states/IGameState.hpp"

namespace gsm {

AGameStateMachine::AGameStateMachine() {}

void AGameStateMachine::changeState(std::shared_ptr<IGameState> newState) {
    if (!states_.empty()) {
        states_.top()->exit();
        states_.pop();
    }
    states_.push(newState);
    states_.top()->enter();
}

void AGameStateMachine::pushState(std::shared_ptr<IGameState> newState) {
    if (!states_.empty()) {
        states_.top()->exit();
    }
    states_.push(newState);
    states_.top()->enter();
}

void AGameStateMachine::popState() {
    if (!states_.empty()) {
        states_.top()->exit();
        states_.pop();
    }
    if (!states_.empty()) {
        states_.top()->enter();
    }
}

void AGameStateMachine::update(float deltaTime) {
    if (!states_.empty()) {
        states_.top()->update(deltaTime);
    }
}

}
