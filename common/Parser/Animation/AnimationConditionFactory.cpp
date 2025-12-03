/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationConditionFactory
*/

#include "AnimationConditionFactory.hpp"
#include <string>
#include <memory>

namespace ecs {

AnimationConditionFactory::AnimationConditionFactory() {
    initializeConditions();
}

const AnimationConditionFactory& AnimationConditionFactory::getInstance() {
    static AnimationConditionFactory instance;
    return instance;
}

void AnimationConditionFactory::registerCondition(
    const std::string& name,
    ConditionFunction condition
) {
    _conditions[name] = condition;
}

bool AnimationConditionFactory::evaluateCondition(
    const std::string& name,
    std::shared_ptr<Registry> registry,
    Entity entity
) const {
    auto it = _conditions.find(name);
    if (it == _conditions.end()) {
        return false;
    }
    return it->second(registry, entity);
}

bool AnimationConditionFactory::hasCondition(const std::string& name) const {
    return _conditions.find(name) != _conditions.end();
}

void AnimationConditionFactory::unregisterCondition(const std::string& name) {
    _conditions.erase(name);
}

void AnimationConditionFactory::clearConditions() {
    _conditions.clear();
}

bool AnimationConditionFactory::getConditionValue(
    const std::string& param,
    std::shared_ptr<Registry> registry,
    Entity entity
) {
    return getInstance().evaluateCondition(param, registry, entity);
}

}  // namespace ecs
