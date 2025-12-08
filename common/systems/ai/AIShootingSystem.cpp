/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AIShootingSystem
*/

#include "AIShootingSystem.hpp"
#include <memory>
#include "../../components/temporary/ShootIntentComponent.hpp"
#include "../../components/tags/AIShooterTag.hpp"

namespace ecs {

void AIShootingSystem::update(
    [[maybe_unused]] std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void) deltaTime;
    auto view = registry->view<AIShooterTag, ShootingStatsComponent, TransformComponent>();
    for (auto entityId : view) {
        if (registry->hasComponent<ShootIntentComponent>(entityId))
            continue;
        auto shootIntentComponent = std::make_shared<ShootIntentComponent>(180.0f);

        registry->addComponent<ShootIntentComponent>(entityId, shootIntentComponent);
    }
}

}  // namespace ecs

