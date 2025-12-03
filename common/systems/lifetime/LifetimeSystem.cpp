/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LifetimeSystem
*/

#include "LifetimeSystem.hpp"
#include <memory>
#include "../../components/permanent/LifetimeComponent.hpp"

namespace ecs {

LifetimeSystem::LifetimeSystem() {
}

void LifetimeSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    auto view = registry->view<LifetimeComponent>();

    for (auto entityId : view) {
        auto lifetimeComponent = registry->getComponent<LifetimeComponent>(entityId);
        if (!lifetimeComponent)
            continue;

        float currentLifetime = lifetimeComponent->getLifetime();
        lifetimeComponent->setLifetime(currentLifetime - deltaTime);

        if (currentLifetime - deltaTime <= 0.0f) {
            registry->destroyEntity(entityId);
        }
    }
}

}
