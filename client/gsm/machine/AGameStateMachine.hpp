#pragma once

#include "../../../common/gsm/IGameStateMachine.hpp"
#include "../../../common/gsm/IGameState.hpp"

namespace gsm {

class AGameStateMachine : public IGameStateMachine {
public:
    AGameStateMachine();
    ~AGameStateMachine() override = default;

    void changeState(std::shared_ptr<IGameState> newState) override;
    void pushState(std::shared_ptr<IGameState> newState) override;
    void popState() override;
    void requestStateChange(std::shared_ptr<IGameState> newState) override;
    void requestStatePush(std::shared_ptr<IGameState> newState) override;
    void requestStatePop() override;

    void update(float deltaTime) override;

protected:
    std::stack<std::shared_ptr<IGameState>> _states;
    std::shared_ptr<IGameState> _pendingChangeState;
    std::shared_ptr<IGameState> _pendingPushState;
    bool _pendingPopState = false;
};

}  // namespace gsm
