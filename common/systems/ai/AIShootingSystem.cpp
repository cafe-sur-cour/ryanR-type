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
#include "../SystemNames.hpp"

namespace ecs {

void AIShootingSystem::update(
    [[maybe_unused]] std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    [[maybe_unused]] float deltaTime
) {
    auto view = registry->view<AIShooterTag, ShootingStatsComponent, TransformComponent>();
    for (auto entityId : view) {
        if (registry->hasComponent<ShootIntentComponent>(entityId))
            continue;
        auto shootingStats = registry->getComponent<ShootingStatsComponent>(entityId);
        float angleOffset = shootingStats ?
            shootingStats->getMultiShotPattern().offsetDistance : 0.0f;
        auto shootIntentComponent = std::make_shared<ShootIntentComponent>(
            180.0f + angleOffset
        );

        registry->addComponent<ShootIntentComponent>(entityId, shootIntentComponent);
    }
}

}  // namespace ecs


extern "C" ecs::ISystem* createSystem() {
    return new ecs::AIShootingSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::AI_SHOOTING_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
