/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootInputSystem
*/

#include "ShootInputSystem.hpp"
#include <memory>
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/components/tags/ShooterTag.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../common/components/permanent/HealthComponent.hpp"
#include "../../../common/components/permanent/ShootingStatsComponent.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../ClientNetwork.hpp"
#include "../../constants.hpp"

namespace ecs {

ShootInputSystem::ShootInputSystem() {
}

void ShootInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    if (!resourceManager->has<IInputProvider>())
        return;

    auto inputProvider = resourceManager->get<IInputProvider>();
    if (!inputProvider->isActionPressed(InputAction::SHOOT))
        return;

    auto playerView =
        registry->view<ControllableTag, ShooterTag, LocalPlayerTag, ShootingStatsComponent>();

    for (auto playerId : playerView) {
        if (!isPlayerAlive(registry, playerId)) {
            continue;
        }
        auto clientNetwork = resourceManager->get<ClientNetwork>();
        if (clientNetwork) {
            NetworkEvent shootEvent;
            shootEvent.eventType = constants::EventType::SHOOT;
            shootEvent.depth = static_cast<double>(1);
            clientNetwork->addToEventQueue(shootEvent);
        }
    }
}

bool ShootInputSystem::isPlayerAlive(
    std::shared_ptr<Registry> registry,
    Entity entityId) const {
    if (registry->hasComponent<HealthComponent>(entityId)) {
        auto health = registry->getComponent<HealthComponent>(entityId);
        return health->getHealth() > 0.0f;
    }
    return true;
}

}
