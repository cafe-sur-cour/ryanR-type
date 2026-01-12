/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ActionFactory
*/

#ifndef ACTIONFACTORY_HPP_
#define ACTIONFACTORY_HPP_

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include "../../ECS/entity/Entity.hpp"
#include "../../resourceManager/ResourceManager.hpp"
#include "TagRegistry.hpp"

namespace ecs {
    class Registry;
}

class ActionFactory {
    public:
        static const ActionFactory& getInstance();

        using ActionFunction = std::function<void(std::shared_ptr<ecs::Registry>, std::shared_ptr<ResourceManager>, ecs::Entity, ecs::Entity)>;

        void registerAction(const std::string& actionId, ActionFunction action);

        void executeAction(
            const std::string& actionId,
            std::shared_ptr<ecs::Registry> registry,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity self, ecs::Entity other) const;

        bool hasAction(const std::string& actionId) const;

    private:
        ActionFactory();
        ~ActionFactory() = default;
        ActionFactory(const ActionFactory&) = delete;
        ActionFactory& operator=(const ActionFactory&) = delete;

        void initializeConditions();

        std::unordered_map<std::string, ActionFunction> _actions;
};

#endif /* !ACTIONFACTORY_HPP_ */
