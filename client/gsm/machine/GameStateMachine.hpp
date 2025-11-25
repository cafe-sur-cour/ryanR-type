#pragma once

#include "AGameStateMachine.hpp"

namespace gsm {

class GameStateMachine : public AGameStateMachine {
public:
    GameStateMachine();
    ~GameStateMachine() override = default;
};

}  // namespace gsm
