/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationConditionsRegistry
*/

#include "AnimationConditionFactory.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../constants.hpp"
#include "../../../client/components/rendering/AnimationComponent.hpp"
#include "../../components/permanent/ShootingStatsComponent.hpp"

namespace ecs {

void AnimationConditionFactory::initializeConditions() {
    registerCondition(constants::VELOCITY_UP_CONDITION,
        [](std::shared_ptr<Registry> registry, Entity entity) -> bool {
            auto velocity = registry->getComponent<VelocityComponent>(entity);
            if (!velocity) return false;
            return velocity->getVelocity().getY() < 0;
        });

    registerCondition(constants::VELOCITY_DOWN_CONDITION,
        [](std::shared_ptr<Registry> registry, Entity entity) -> bool {
            auto velocity = registry->getComponent<VelocityComponent>(entity);
            if (!velocity) return false;
            return velocity->getVelocity().getY() > 0;
        });

    registerCondition(constants::IS_ANIMATION_FINISHED,
        [](std::shared_ptr<Registry> registry, Entity entity) -> bool {
            auto animation = registry->getComponent<ecs::AnimationComponent>(entity);
            if (!animation) return false;
            return animation->isAnimationFinished();
        });

    registerCondition(constants::IS_SHOOTING_CONDITION,
        [](std::shared_ptr<Registry> registry, Entity entity) -> bool {
            auto shootingStats = registry->getComponent<ShootingStatsComponent>(entity);
            if (!shootingStats) return false;
            return shootingStats->isShooting();
        });
}

}  // namespace ecs
