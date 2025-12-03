/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationConditionFactory
*/

#ifndef ANIMATIONCONDITIONFACTORY_HPP_
#define ANIMATIONCONDITIONFACTORY_HPP_

#include <functional>
#include <string>
#include "../../ECS/entity/Entity.hpp"
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../constants.hpp"

namespace ecs {

class AnimationConditionFactory {
    public:
        static std::function<bool(std::shared_ptr<Registry>, Entity)> getCondition(const std::string& key) {
            if (key == constants::VELOCITY_UP_CONDITION) {
                return [](std::shared_ptr<Registry> registry, Entity entity) {
                    auto velocity = registry->getComponent<VelocityComponent>(entity);
                    if (!velocity) return false;
                    return velocity->getVelocity().getY() < 0;
                };
            } else if (key == constants::VELOCITY_DOWN_CONDITION) {
                return [](std::shared_ptr<Registry> registry, Entity entity) {
                    auto velocity = registry->getComponent<VelocityComponent>(entity);
                    if (!velocity) return false;
                    return velocity->getVelocity().getY() >= 0;
                };
            }
            return [](std::shared_ptr<Registry>, Entity) { return false; };
        }
};

}  // namespace ecs

#endif /* !ANIMATIONCONDITIONFACTORY_HPP_ */