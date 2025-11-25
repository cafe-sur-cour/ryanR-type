#include "AGameState.hpp"

namespace gsm {

AGameState::AGameState(std::shared_ptr<IGameStateMachine> gsm) {
    _gsm = gsm;
}

void AGameState::enter() {
}

void AGameState::update(float deltaTime) {
    (void)deltaTime;
}

void AGameState::exit() {
}

}  // namespace gsm
