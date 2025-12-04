/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ActionFactory
*/

#include "ActionFactory.hpp"
#include <memory>
#include <string>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../constants.hpp"

const ActionFactory& ActionFactory::getInstance() {
    static ActionFactory instance;
    return instance;
}

ActionFactory::ActionFactory() {
    initializeConditions();
}

void ActionFactory::registerAction(const std::string& actionId, ActionFunction action) {
    _actions[actionId] = action;
}

void ActionFactory::executeAction(
    const std::string& actionId,
    std::shared_ptr<ecs::Registry> registry,
    ecs::Entity self,
    ecs::Entity other
) const {
    auto it = _actions.find(actionId);
    if (it != _actions.end()) {
        it->second(registry, self, other);
    }
}

bool ActionFactory::hasAction(const std::string& actionId) const {
    return _actions.find(actionId) != _actions.end();
}
