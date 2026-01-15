/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthSystem
*/

#include "HealthSystem.hpp"
#include <memory>
#include "../../components/permanent/HealthComponent.hpp"
#include "../../components/permanent/InvulnerableComponent.hpp"
#include "../../components/temporary/DamageIntentComponent.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../GameRules.hpp"
#include "../../resourceManager/ResourceManager.hpp"
#include "../../constants.hpp"
#include "../interactions/TagRegistry.hpp"

namespace ecs {

HealthSystem::HealthSystem() {
}

void HealthSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    _handleDamageUpdates(resourceManager, registry);
    _handleHealthUpdates(registry);
}

void HealthSystem::_handleDamageUpdates(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry
) {
    auto view = registry->view<HealthComponent, DamageIntentComponent>();

    for (auto entityId : view) {
        auto healthComponent = registry->getComponent<HealthComponent>(entityId);
        auto damageComponent = registry->getComponent<DamageIntentComponent>(entityId);

        if (registry->hasComponent<InvulnerableComponent>(entityId)) {
            auto invulnerableComponent = registry->getComponent
                <InvulnerableComponent>(entityId);
            if (invulnerableComponent->isActive()) {
                registry->removeOneComponent<DamageIntentComponent>(entityId);
                continue;
            }
        }

        float damages = damageComponent->getDamages();
        auto gameRules = resourceManager->get<GameRules>();
        if (gameRules) {
            GameRulesNS::Difficulty diff = gameRules->getDifficulty();
            float multiplier = constants::DIFFICULTY_NORMAL_MULTIPLIER;
            if (diff == GameRulesNS::Difficulty::EASY) multiplier =
                constants::DIFFICULTY_EASY_MULTIPLIER;
            else if (diff == GameRulesNS::Difficulty::HARD) multiplier =
                constants::DIFFICULTY_HARD_MULTIPLIER;
            bool isPlayer = TagRegistry::getInstance().hasTag(
                registry, entityId, constants::PLAYERTAG);
            bool isMob = TagRegistry::getInstance().hasTag(
                registry, entityId, constants::MOBTAG);
            if (isPlayer) {
                damages *= (2.0f - multiplier);
            } else if (isMob) {
                damages *= multiplier;
            }
        }

        healthComponent->setHealth(healthComponent->getHealth() - damages);
        healthComponent->setLastDamageSource(damageComponent->getSource());

        registry->removeOneComponent<DamageIntentComponent>(entityId);
    }
}

void HealthSystem::_handleHealthUpdates(std::shared_ptr<Registry> registry) {
    auto view = registry->view<HealthComponent>();

    for (auto entityId : view) {
        auto healthComponent = registry->getComponent<HealthComponent>(entityId);

        if (healthComponent->getHealth() <= 0.0f) {
            ecs::Entity source = healthComponent->getLastDamageSource();
            registry->addComponent(entityId, std::make_shared<DeathIntentComponent>(source));
        }
    }
}

}
