/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthSystem
*/

#include "HealthSystem.hpp"
#include <memory>
#include "../../components/permanent/HealthComponent.hpp"
#include "../../components/temporary/DamageIntentComponent.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"

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

    _handleDamageUpdates(registry);
    _handleHealthUpdates(registry);
}

void HealthSystem::_handleDamageUpdates(std::shared_ptr<Registry> registry) {
    auto view = registry->view<HealthComponent, DamageIntentComponent>();

    for (auto entityId : view) {
        auto healthComponent = registry->getComponent<HealthComponent>(entityId);
        auto damageComponent = registry->getComponent<DamageIntentComponent>(entityId);

        float damages = damageComponent->getDamages();
        float health = healthComponent->getHealth();

        healthComponent->setHealth(health - damages);

        registry->removeComponent<DamageIntentComponent>(entityId);
    }
}

void HealthSystem::_handleHealthUpdates(std::shared_ptr<Registry> registry) {
    auto view = registry->view<HealthComponent>();

    for (auto entityId : view) {
        auto healthComponent = registry->getComponent<HealthComponent>(entityId);

        if (healthComponent->getHealth() <= 0.0f) {
            registry->addComponent(entityId, std::make_shared<DeathIntentComponent>());
        }
    }
}

}
