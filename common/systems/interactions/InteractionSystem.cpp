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
    (void)deltaTime;

    auto triggerIntentView = registry->view<TriggerIntentComponent>();
    for (auto entity : triggerIntentView) {
        auto triggerIntent = registry->getComponent<TriggerIntentComponent>(entity);
        if (!triggerIntent) continue;

        Entity otherEntity = triggerIntent->getOther();

        auto interactionConfig = registry->getComponent<InteractionConfigComponent>(entity);
        if (!interactionConfig) {
            registry->removeComponent<TriggerIntentComponent>(entity);
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

        if (shouldInteract) {
            for (const auto& action : actionsToOther) {
                if (!action.empty()) {
                    ActionFactory::getInstance().executeAction(action,
                        registry, entity, otherEntity);
                }
            }

            for (const auto& action : actionsToSelf) {
                if (!action.empty()) {
                    ActionFactory::getInstance().executeAction(action,
                        registry, entity, otherEntity);
                }
            }
        }

        registry->removeComponent<TriggerIntentComponent>(entity);
    }
}

}  // namespace ecs
