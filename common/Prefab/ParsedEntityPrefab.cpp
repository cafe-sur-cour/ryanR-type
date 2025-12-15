/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParsedEntityPrefab
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "ParsedEntityPrefab.hpp"
#include "../Parser/ParserParam.hpp"
#include "../Error/ParserError.hpp"
#include "../ECS/entity/factory/EntityFactory.hpp"

ParsedEntityPrefab::ParsedEntityPrefab(const std::string& name, const std::map<std::type_index,
    ComponentAdder>& adders)
    : _name(name), _componentAdders(adders) {
    this->_components = {};
}

ParsedEntityPrefab::~ParsedEntityPrefab() {
}

void ParsedEntityPrefab::addComponent(std::shared_ptr<ecs::IComponent> component,
    std::type_index typeIndex) {
    _components.push_back({component, typeIndex});
}

const std::vector<std::shared_ptr<ecs::IComponent>>& ParsedEntityPrefab::getComponents()
    const {
    static std::vector<std::shared_ptr<ecs::IComponent>> temp;
    temp.clear();
    for (const auto& [comp, _] : _components) {
        temp.push_back(comp);
    }
    return temp;
}

std::string ParsedEntityPrefab::getName() const {
    return _name;
}

void ParsedEntityPrefab::addParsedComponents(
    const std::shared_ptr<ecs::Registry>& registry,
    ecs::Entity entity
) {
    for (const auto& [component, typeIndex] : _components) {
        auto it = _componentAdders.find(typeIndex);
        if (it != _componentAdders.end()) {
            it->second(registry, entity, component);
        } else {
            throw err::ParserError(std::string("Unknown component type: ") +
            typeIndex.name(), err::ParserError::UNKNOWN);
        }
    }
}

ecs::Entity ParsedEntityPrefab::instantiate(
    const std::shared_ptr<ecs::Registry>& registry,
    const std::shared_ptr<ecs::IEntityFactory>& factory,
    const ecs::EntityCreationContext& context
) {
    ecs::Entity entity = factory->createEntity(registry, context);
    addParsedComponents(registry, entity);
    return entity;
}

ecs::Entity ParsedEntityPrefab::instantiate(
    const std::shared_ptr<ecs::Registry>& registry) {
    ecs::Entity entity = registry->createEntity();
    addParsedComponents(registry, entity);
    return entity;
}
