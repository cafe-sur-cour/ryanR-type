/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionSystem
*/

#include "InteractionSystem.hpp"
#include <memory>
#include <string>
#include <vector>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/temporary/TriggerIntentComponent.hpp"
#include "../../components/permanent/InteractionConfigComponent.hpp"
#include "../../components/permanent/DamageCooldownComponent.hpp"
#include "../../components/permanent/OwnerComponent.hpp"
#include "../../components/tags/PlayerProjectileTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../GameRules.hpp"
#include "../../constants.hpp"
#include "ActionFactory.hpp"
#include "TagRegistry.hpp"

namespace ecs {

InteractionSystem::InteractionSystem() {
}

void InteractionSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;

    auto cooldownView = registry->view<DamageCooldownComponent>();
    for (auto entity : cooldownView) {
        auto cooldownComp = registry->getComponent<DamageCooldownComponent>(entity);
        if (cooldownComp) {
            cooldownComp->setLastDamageTime(cooldownComp->getLastDamageTime() + deltaTime);
        }
    }

    auto triggerIntentView = registry->view<TriggerIntentComponent>();
    for (auto entity : triggerIntentView) {
        auto triggerIntent = registry->getComponent<TriggerIntentComponent>(entity);
        if (!triggerIntent) continue;

        Entity otherEntity = triggerIntent->getOther();

        auto interactionConfig = registry->getComponent<InteractionConfigComponent>(entity);
        if (!interactionConfig) {
            registry->removeOneComponent<TriggerIntentComponent>(entity);
            continue;
        }

        bool shouldInteract = false;
        std::vector<std::string> actionsToOther;
        std::vector<std::string> actionsToSelf;

        for (const auto& mapping : interactionConfig->getMappings()) {
            bool hasRequiredTag = false;
            for (const auto& targetTag : mapping.targetTags) {
                if (TagRegistry::getInstance().hasTag(registry, otherEntity, targetTag)) {
                    hasRequiredTag = true;
                    break;
                }
            }

            if (hasRequiredTag) {
                shouldInteract = true;
                actionsToOther = mapping.actionsToOther;
                actionsToSelf = mapping.actionsToSelf;
                break;
            }
        }

        if (resourceManager && resourceManager->has<GameRules>()) {
            auto gameRules = resourceManager->get<GameRules>();
            if (gameRules && gameRules->getCrossfire()) {
                bool isPlayerProjectile =
                    registry->hasComponent<PlayerProjectileTag>(entity);
                bool isPlayerTarget = registry->hasComponent<PlayerTag>(otherEntity);
                bool reversePlayerProjectile =
                    registry->hasComponent<PlayerProjectileTag>(otherEntity);
                bool reversePlayerTarget = registry->hasComponent<PlayerTag>(entity);

                if (isPlayerProjectile && isPlayerTarget) {
                    auto ownerComp = registry->getComponent<OwnerComponent>(entity);
                    if (!ownerComp || ownerComp->getOwner() != otherEntity) {
                        ActionFactory::getInstance().executeAction(
                            constants::DEALDAMAGE_ACTION,
                            registry, resourceManager, entity, otherEntity);
                        ActionFactory::getInstance().executeAction(
                            constants::TAKEDEATH_ACTION,
                            registry, resourceManager, entity, otherEntity);
                        shouldInteract = true;
                    }
                } else if (reversePlayerProjectile && reversePlayerTarget) {
                    auto ownerComp =
                        registry->getComponent<OwnerComponent>(otherEntity);
                    if (!ownerComp || ownerComp->getOwner() != entity) {
                        ActionFactory::getInstance().executeAction(
                            constants::DEALDAMAGE_ACTION,
                            registry, resourceManager, otherEntity, entity);
                        ActionFactory::getInstance().executeAction(
                            constants::TAKEDEATH_ACTION,
                            registry, resourceManager, otherEntity, entity);
                        shouldInteract = true;
                    }
                }
            }
        }

        if (shouldInteract) {
            auto filteredActionsToOther =
                filterDamageActions(actionsToOther, registry, otherEntity);
            auto filteredActionsToSelf =
                filterDamageActions(actionsToSelf, registry, entity);

            for (const auto& action : filteredActionsToOther) {
                if (!action.empty()) {
                    ActionFactory::getInstance().executeAction(action,
                        registry, resourceManager, entity, otherEntity);
                }
            }

            for (const auto& action : filteredActionsToSelf) {
                if (!action.empty()) {
                    ActionFactory::getInstance().executeAction(action,
                        registry, resourceManager, entity, otherEntity);
                }
            }
        }

        registry->removeOneComponent<TriggerIntentComponent>(entity);
    }
}

std::vector<std::string> InteractionSystem::filterDamageActions(
    const std::vector<std::string>& actions,
    std::shared_ptr<Registry> registry,
    Entity targetEntity
) {
    std::vector<std::string> filtered;
    for (const auto& action : actions) {
        if (action == constants::DEALDAMAGE_ACTION ||
            action == constants::TAKEDAMAGE_ACTION) {
            auto cooldownComp =
                registry->getComponent<DamageCooldownComponent>(targetEntity);
            if (cooldownComp &&
                cooldownComp->getLastDamageTime() < cooldownComp->getCooldown()) {
                continue;
            }
        }
        filtered.push_back(action);
    }
    return filtered;
}

}  // namespace ecs
