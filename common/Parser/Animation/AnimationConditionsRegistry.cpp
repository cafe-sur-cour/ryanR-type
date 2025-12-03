/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationConditionsRegistry
*/

#include "AnimationConditionFactory.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../constants.hpp"

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
}

}  // namespace ecs
