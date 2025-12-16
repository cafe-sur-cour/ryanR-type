/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EndOfMapDetectionSystem
*/

#include "EndOfMapDetectionSystem.hpp"
#include <iostream>
#include <memory>
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../gsm/gsmStates.hpp"
#include "../../../common/components/tags/GameEndTag.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/ColliderComponent.hpp"

namespace ecs {

EndOfMapDetectionSystem::EndOfMapDetectionSystem() {
}

EndOfMapDetectionSystem::~EndOfMapDetectionSystem() {
}

bool collides(const TransformComponent& t1, const ColliderComponent& c1,
              const TransformComponent& t2, const ColliderComponent& c2) {
    float left1 = t1.getPosition().getX() + c1.getOffset().getX();
    float right1 = left1 + c1.getSize().getX();
    float top1 = t1.getPosition().getY() + c1.getOffset().getY();
    float bottom1 = top1 + c1.getSize().getY();

    float left2 = t2.getPosition().getX() + c2.getOffset().getX();
    float right2 = left2 + c2.getSize().getX();
    float top2 = t2.getPosition().getY() + c2.getOffset().getY();
    float bottom2 = top2 + c2.getSize().getY();

    return !(right1 <= left2 || right2 <= left1 || bottom1 <= top2 || bottom2 <= top1);
}

void EndOfMapDetectionSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)deltaTime;

    auto players = registry->view<ecs::PlayerTag>();

    bool hasPlayers = false;
    for (auto p : players) {
        (void)p;
        hasPlayers = true;
        break;
    }
    if (!hasPlayers) {
        *resourceManager->get<gsm::GameStateType>() = gsm::GAME_END;
        return;
    }
    auto gameEndTags = registry->view<GameEndTag>();

    for (auto playerId : players) {
        auto playerTransform = registry->getComponent<TransformComponent>(playerId);
        auto playerCollider = registry->getComponent<ColliderComponent>(playerId);
        if (!playerTransform || !playerCollider) continue;

        for (auto endEntityId : gameEndTags) {
            auto endTransform = registry->getComponent<TransformComponent>(endEntityId);
            auto endCollider = registry->getComponent<ColliderComponent>(endEntityId);
            if (!endTransform || !endCollider) {
                continue;
            }

            if (collides(*playerTransform, *playerCollider, *endTransform, *endCollider)) {
                if (resourceManager->has<gsm::GameStateType>()) {
                    *resourceManager->get<gsm::GameStateType>() = gsm::GAME_END;
                }
                return;
            }
        }
    }
}

}  // namespace ecs

