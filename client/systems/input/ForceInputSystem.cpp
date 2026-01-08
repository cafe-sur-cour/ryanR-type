/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ForceInputSystem
*/

#include "ForceInputSystem.hpp"
#include <memory>
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../common/components/permanent/HealthComponent.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../ClientNetwork.hpp"
#include "../../constants.hpp"

namespace ecs {

ForceInputSystem::ForceInputSystem() : _lastForceTime(0.0f) {
}

void ForceInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    _lastForceTime += deltaTime;

    if (!resourceManager->has<IInputProvider>())
        return;

    auto inputProvider = resourceManager->get<IInputProvider>();
    if (!inputProvider->isActionPressed(InputAction::FORCE))
        return;
    if (_lastForceTime < 0.3f)
        return;

    auto playerView = registry->view<ControllableTag, LocalPlayerTag>();

    for (auto playerId : playerView) {
        if (!isPlayerAlive(registry, playerId)) {
            continue;
        }

        auto clientNetwork = resourceManager->get<ClientNetwork>();
        if (clientNetwork) {
            NetworkEvent forceEvent;
            forceEvent.eventType = constants::EventType::FORCE;
            forceEvent.depth = static_cast<double>(1);
            clientNetwork->addToEventQueue(forceEvent);
            _lastForceTime = 0.0f;
        }
    }
}

bool ForceInputSystem::isPlayerAlive(
    std::shared_ptr<Registry> registry,
    Entity entityId) const {
    if (registry->hasComponent<HealthComponent>(entityId)) {
        auto health = registry->getComponent<HealthComponent>(entityId);
        return health->getHealth() > 0.0f;
    }
    return true;
}

}  // namespace ecs
