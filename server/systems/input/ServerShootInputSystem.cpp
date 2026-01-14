/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerShootInputSystem
*/

#include <memory>
#include <vector>
#include "ServerShootInputSystem.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/components/tags/ShooterTag.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../../common/components/permanent/ShootingStatsComponent.hpp"
#include "../../constants.hpp"

namespace ecs {

ServerShootInputSystem::ServerShootInputSystem() {
}

void ServerShootInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {

    if (!resourceManager->has<ecs::ServerInputProvider>()) {
        return;
    }

    auto serverInputProvider = resourceManager->get<ecs::ServerInputProvider>();

    auto view = registry->view<ControllableTag, ShooterTag, PlayerTag>();

    for (auto entityId : view) {
        if (!_cooldowns.contains(entityId))
            _cooldowns[entityId] = 0;
        _cooldowns[entityId] += deltaTime;

        size_t clientID = serverInputProvider->getClientIdForEntity(entityId);
        if (clientID == 0) {
            continue;
        }

        float value = serverInputProvider->getActionAxis(InputAction::SHOOT, clientID);
        if (value <= 0.0f)
            continue;

        float cooldown = constants::SHOOT_INPUT_COOLDOWN;
        if (registry->hasComponent<ShootingStatsComponent>(entityId)) {
            cooldown = 1.0f /
                registry->getComponent<ShootingStatsComponent>(entityId)->getFireRate();
        }

        if (_cooldowns[entityId] > cooldown) {
            _cooldowns[entityId] = 0.0f;
            updateShootIntent(registry, entityId);
        }
        serverInputProvider->setAxisValue(InputAction::SHOOT, 0.0f, clientID);
    }
}

void ServerShootInputSystem::updateShootIntent(
    std::shared_ptr<Registry> registry,
    Entity entityId
) {
    registry->registerComponent<ShootIntentComponent>();

    if (!registry->hasComponent<ShootIntentComponent>(entityId)) {
        float angle = 0.0f;

        if (registry->hasComponent<ShootingStatsComponent>(entityId)) {
            angle = registry->getComponent<ShootingStatsComponent>(
                entityId
            )->getMultiShotPattern().angleOffset;
        }

        auto shootIntentComponent = std::make_shared<ShootIntentComponent>(angle);
        registry->addComponent(entityId, shootIntentComponent);
    }
}

}  // namespace ecs
