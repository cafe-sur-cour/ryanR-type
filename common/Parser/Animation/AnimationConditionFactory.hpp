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
#include <unordered_map>
#include <memory>
#include "../../ECS/entity/Entity.hpp"
#include "../../ECS/entity/registry/Registry.hpp"

namespace ecs {

class AnimationConditionFactory {
    public:
        using ConditionFunction = std::function<bool(std::shared_ptr<Registry>, Entity)>;

        static const AnimationConditionFactory& getInstance();

        void registerCondition(const std::string& name, ConditionFunction condition);
        bool evaluateCondition(const std::string& name, std::shared_ptr<Registry> registry, Entity entity) const;
        bool hasCondition(const std::string& name) const;
        void unregisterCondition(const std::string& name);
        void clearConditions();

        static bool getConditionValue(const std::string& param, std::shared_ptr<Registry> registry, Entity entity);

    private:
        AnimationConditionFactory();
        void initializeConditions();

        AnimationConditionFactory(const AnimationConditionFactory&) = delete;
        AnimationConditionFactory& operator=(const AnimationConditionFactory&) = delete;

        std::unordered_map<std::string, ConditionFunction> _conditions;
};

}  // namespace ecs

#endif /* !ANIMATIONCONDITIONFACTORY_HPP_ */
