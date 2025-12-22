/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LifetimeSystem
*/

#include "LifetimeSystem.hpp"
#include <memory>
#include "../../components/permanent/LifetimeComponent.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../SystemNames.hpp"

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
            registry->addComponent(entityId, std::make_shared<DeathIntentComponent>());
        }
    }
}

}

extern "C" ecs::ISystem* createSystem() {
    return new ecs::LifetimeSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::LIFETIME_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
