#pragma once

#include "IGameState.hpp"

namespace gsm {

class AGameState : public IGameState {
public:
    AGameState(std::shared_ptr<IGameStateMachine> gsm);
    ~AGameState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void render() override;
    void exit() override;

protected:
    std::shared_ptr<IGameStateMachine> _gsm;
};

}
