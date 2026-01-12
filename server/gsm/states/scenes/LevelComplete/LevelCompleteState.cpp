/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelCompleteState
*/

#include "LevelCompleteState.hpp"
#include <memory>
#include <string>
#include "../../../machine/GameStateMachine.hpp"
#include "../../../../Server.hpp"
#include "../../../../../common/Parser/MapParser/MapHandler.hpp"
#include "../../../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/ECS/entity/EntityCreationContext.hpp"
#include "../InGame/InGameState.hpp"
#include "../GameEnd/GameEndState.hpp"
#include "../../../gsmStates.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../../common/components/permanent/ScoreComponent.hpp"

namespace gsm {

LevelCompleteState::LevelCompleteState(std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager), _transitionTimer(0.0f) {
}

void LevelCompleteState::enter() {
    *(_resourceManager->get<gsm::GameStateType>()) = gsm::GameStateType::LEVEL_COMPLETE;
    _transitionTimer = 0.0f;
}

void LevelCompleteState::update(float deltaTime) {
    _transitionTimer += deltaTime;

    if (_transitionTimer >= TRANSITION_DELAY) {
        auto mapHandler = _resourceManager->get<MapHandler>();

        bool hasNextMap = mapHandler && mapHandler->advanceToNextMap();

        if (hasNextMap) {
            auto registry = _resourceManager->get<ecs::Registry>();

            _savedPlayerScores.clear();
            if (registry) {
                auto playerView = registry->view<ecs::PlayerTag>();
                for (auto entity : playerView) {
                    if (registry->hasComponent<ecs::ScoreComponent>(entity)) {
                        auto scoreComp = registry->getComponent<ecs::ScoreComponent>(entity);
                        _savedPlayerScores.push_back(scoreComp->getScore());
                    } else {
                        _savedPlayerScores.push_back(0);
                    }
                }
            }

            if (registry) {
                registry->clearAllEntities();
            }

            auto lobby = _resourceManager->get<rserv::Lobby>();

            lobby->clearDeltaTrackerCaches();

            lobby->nextLevelPacket();

            auto prefabMgr = _resourceManager->get<EntityPrefabManager>();
            if (lobby && prefabMgr && registry) {
                std::string playerPrefab = "player";
                auto clientIds = lobby->getConnectedClients();
                size_t playerIndex = 0;
                for (size_t i = 0; i < clientIds.size(); i++) {
                    auto newEntity = prefabMgr->createEntityFromPrefab(
                        playerPrefab,
                        registry,
                        ecs::EntityCreationContext::forServer()
                    );

                    if (playerIndex < _savedPlayerScores.size()) {
                        if (registry->hasComponent<ecs::ScoreComponent>(newEntity)) {
                            registry->getComponent<ecs::ScoreComponent>(newEntity)
                                ->setScore(_savedPlayerScores[playerIndex]);
                        }
                    }
                    playerIndex++;
                }
            }

            if (auto gsmPtr = _gsm.lock()) {
                if (auto gsm = std::dynamic_pointer_cast<GameStateMachine>(gsmPtr)) {
                    *(_resourceManager->get<gsm::GameStateType>()) =
                        gsm::GameStateType::IN_GAME;
                    gsm->requestStateChange(std::make_shared<gsm::InGameState>
                        (gsmPtr, _resourceManager));
                }
            }
        } else {
            if (auto gsmPtr = _gsm.lock()) {
                if (auto gsm = std::dynamic_pointer_cast<GameStateMachine>(gsmPtr)) {
                    *(_resourceManager->get<gsm::GameStateType>()) =
                        gsm::GameStateType::GAME_END;
                    gsm->requestStateChange(std::make_shared<gsm::GameEndState>
                        (gsmPtr, _resourceManager));
                }
            }
        }
    }
}

}  // namespace gsm
