/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthSystem
*/

#include "HealthSystem.hpp"
#include <memory>
#include "../../components/permanent/HealthComponent.hpp"

namespace ecs {

HealthSystem::HealthSystem() {
}

void HealthSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) resourceManager;
    (void) deltaTime;

    auto view = registry->view<HealthComponent>();

    for (auto entityId : view) {
        auto healthComponent = registry->getComponent<HealthComponent>(entityId);

        if (healthComponent->getHealth() <= 0.0f) {
            registry->destroyEntity(entityId);
        }
    }
}

}
