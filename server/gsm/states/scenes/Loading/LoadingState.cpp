/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LoadingState
*/

#include "LoadingState.hpp"
#include <memory>
#include "../InGame/InGameState.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"

namespace gsm {

LoadingState::LoadingState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void LoadingState::enter() {
    auto parser = _resourceManager->get<Parser>();
    if (parser) {
        parser->parseMapFromFile("configs/map/map1.json");
    }
    _gsm->requestStateChange(std::make_shared<InGameState>(_gsm, _resourceManager));
}


}  // namespace gsm
