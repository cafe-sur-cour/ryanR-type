/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathSystem
*/

#include "DeathSystem.hpp"
#include <memory>
#include "../../components/temporary/DeathIntentComponent.hpp"

namespace ecs {

DeathSystem::DeathSystem() {
}

void DeathSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) resourceManager;
    (void) deltaTime;

    auto view = registry->view<DeathIntentComponent>();

    for (auto entityId : view) {
        registry->destroyEntity(entityId);
    }
}

}
