/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionSystem
*/

#include "InteractionSystem.hpp"
#include <memory>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/temporary/TriggerIntentComponent.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../components/tags/ProjectileTag.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../Parser/Action/ActionFactory.hpp"
#include "../../constants.hpp"

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

        bool isProjectile = registry->hasComponent<ProjectileTag>(entity);
        bool isMob = registry->hasComponent<MobTag>(otherEntity);

        if (isProjectile && isMob) {
            ActionFactory::getInstance().executeAction(
                constants::DEATHINTENT_ACTION, registry, entity, otherEntity);
        }

        registry->removeComponent<TriggerIntentComponent>(entity);
    }
}

}  // namespace ecs
