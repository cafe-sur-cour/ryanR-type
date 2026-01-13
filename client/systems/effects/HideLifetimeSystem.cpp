/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HideLifetimeSystem
*/

#include "HideLifetimeSystem.hpp"
#include <memory>
#include "../../common/components/permanent/LifetimeComponent.hpp"
#include "../components/rendering/SpriteComponent.hpp"
#include "../components/rendering/AnimationComponent.hpp"

namespace ecs {

HideLifetimeSystem::HideLifetimeSystem() {
}

void HideLifetimeSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) resourceManager;

    auto view = registry->view<LifetimeComponent>();

    for (auto entityId : view) {
        auto lifetimeComponent = registry->getComponent<LifetimeComponent>(entityId);
        if (!lifetimeComponent) continue;

        float currentLifetime = lifetimeComponent->getLifetime();
        lifetimeComponent->setLifetime(currentLifetime - deltaTime);

        if (currentLifetime - deltaTime <= 0.0f) {
            if (registry->hasComponent<SpriteComponent>(entityId)) {
                registry->removeOneComponent<SpriteComponent>(entityId);
            }
            if (registry->hasComponent<AnimationComponent>(entityId)) {
                registry->removeOneComponent<AnimationComponent>(entityId);
            }
        }
    }
}

}  // namespace ecs
