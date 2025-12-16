#pragma once

#include "../../../common/gsm/IGameStateMachine.hpp"
#include "../../../common/gsm/IGameState.hpp"

namespace gsm {

class AGameStateMachine : public IGameStateMachine {
public:
    AGameStateMachine();
    ~AGameStateMachine() override;

    void changeState(std::shared_ptr<IGameState> newState) override;
    void pushState(std::shared_ptr<IGameState> newState) override;
    void popState() override;

    void update(float deltaTime) override;

    void requestStateChange(std::shared_ptr<IGameState> newState) override;

protected:
    std::stack<std::shared_ptr<IGameState>> _states;
};

} // namespace gsm
