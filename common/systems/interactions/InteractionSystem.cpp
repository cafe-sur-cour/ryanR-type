/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionSystem
*/

#include "InteractionSystem.hpp"
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/temporary/TriggerIntentComponent.hpp"
#include "../../components/permanent/InteractionConfigComponent.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../components/tags/ProjectileTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../Parser/Action/ActionFactory.hpp"
#include "../../constants.hpp"

namespace ecs {

InteractionSystem::InteractionSystem() {
}

bool InteractionSystem::entityHasTag(
    std::shared_ptr<Registry> registry,
    ecs::Entity entity,
    const std::string& tagName
) {
    static const std::unordered_map<std::string,
        std::function<bool(std::shared_ptr<Registry>, ecs::Entity)>> tagCheckers = {
        {constants::MOBTAG,
            [](std::shared_ptr<Registry> reg, ecs::Entity ent)
                { return reg->hasComponent<MobTag>(ent); }},
        {constants::PROJECTILETAG,
            [](std::shared_ptr<Registry> reg, ecs::Entity ent)
                { return reg->hasComponent<ProjectileTag>(ent); }},
        {constants::PLAYERTAG,
            [](std::shared_ptr<Registry> reg, ecs::Entity ent)
                { return reg->hasComponent<PlayerTag>(ent); }},
    };

    auto it = tagCheckers.find(tagName);
    if (it != tagCheckers.end()) {
        return it->second(registry, entity);
    }

    return false;
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
        std::string actionToOther;
        std::string actionToSelf;

        for (const auto& mapping : interactionConfig->getMappings()) {
            bool hasRequiredTag = false;
            for (const auto& targetTag : mapping.targetTags) {
                if (entityHasTag(registry, otherEntity, targetTag)) {
                    hasRequiredTag = true;
                    break;
                }
            }

            if (hasRequiredTag) {
                shouldInteract = true;
                actionToOther = mapping.actionToOther;
                actionToSelf = mapping.actionToSelf;
                break;
            }
        }

        if (shouldInteract) {
            if (!actionToOther.empty()) {
                ActionFactory::getInstance().executeAction(actionToOther,
                    registry, entity, otherEntity);
            }

            if (!actionToSelf.empty()) {
                ActionFactory::getInstance().executeAction(actionToSelf,
                    registry, entity, otherEntity);
            }
        }

        registry->removeComponent<TriggerIntentComponent>(entity);
    }
}

}  // namespace ecs
