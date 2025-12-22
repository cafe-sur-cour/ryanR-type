/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientEffectCleanupSystem
*/

#include "ClientEffectCleanupSystem.hpp"
#include <memory>
#include "../../../common/components/permanent/LifetimeComponent.hpp"
#include "../../../common/components/tags/ClientEffectTag.hpp"
#include "../../../common/systems/SystemNames.hpp"

namespace ecs {

ClientEffectCleanupSystem::ClientEffectCleanupSystem() {
}

void ClientEffectCleanupSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) resourceManager;

    auto view = registry->view<ClientEffectTag, LifetimeComponent>();

    for (auto entityId : view) {
        auto lifetimeComponent = registry->getComponent<LifetimeComponent>(entityId);
        if (!lifetimeComponent) continue;

        float currentLifetime = lifetimeComponent->getLifetime();
        lifetimeComponent->setLifetime(currentLifetime - deltaTime);

        if (currentLifetime - deltaTime <= 0.0f) {
            registry->destroyEntity(entityId);
        }
    }
}

}

extern "C" ecs::ISystem* createSystem() {
    return new ecs::ClientEffectCleanupSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::CLIENT_EFFECT_CLEANUP_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
