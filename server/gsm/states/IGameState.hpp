#pragma once

#include <memory>
#include "../machine/IGameStateMachine.hpp"

namespace gsm {

class IGameState {
public:
    virtual ~IGameState() = default;

    virtual void enter() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void exit() = 0;
};

} // namespace gsm
