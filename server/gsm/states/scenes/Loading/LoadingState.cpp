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
#include "../../../../../common/Parser/MapParser/MapHandler.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../gsmStates.hpp"
namespace gsm {

LoadingState::LoadingState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager) {
}

void LoadingState::enter() {
    auto mapHandler = _resourceManager->get<MapHandler>();
    if (!mapHandler) {
        mapHandler = std::make_shared<MapHandler>();
        mapHandler->parseAllLevels("configs/map");
        _resourceManager->add<MapHandler>(mapHandler);
    }

    auto parser = _resourceManager->get<Parser>();
    if (parser && mapHandler && mapHandler->getTotalMaps() > 0) {
        nlohmann::json currentMap = mapHandler->getCurrentMapJson();
        parser->parseMapFromJson(currentMap);
    }
    *(_resourceManager->get<gsm::GameStateType>()) = gsm::LOADING;
    if (auto stateMachine = _gsm.lock()) {
        stateMachine->requestStateChange(std::make_shared<InGameState>(stateMachine,
            _resourceManager));
    }
}


}  // namespace gsm
