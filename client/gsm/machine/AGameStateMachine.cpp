#include "AGameStateMachine.hpp"
#include <stack>
#include <vector>
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
    if (_states.empty()) {
        return;
    }

    std::vector<std::shared_ptr<IGameState>> statesToUpdate;
    std::stack<std::shared_ptr<IGameState>> tempStack = _states;

    while (!tempStack.empty()) {
        statesToUpdate.push_back(tempStack.top());
        tempStack.pop();
    }

    for (auto it = statesToUpdate.rbegin(); it != statesToUpdate.rend(); ++it) {
        (*it)->update(deltaTime);
    }
}

}  // namespace gsm
