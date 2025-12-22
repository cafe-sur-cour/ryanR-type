/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityFactory
*/

#include "EntityFactory.hpp"
#include <memory>

namespace ecs {

EntityFactory::EntityFactory()
    : _nextLocalId(0) {}

EntityFactory::~EntityFactory() = default;

Entity EntityFactory::createEntity(
    const std::shared_ptr<Registry>& registry,
    const EntityCreationContext& context
) {
    Entity entity;

    if (context.origin == EntityCreationOrigin::CLIENT_LOCAL) {
        if (_nextLocalId.load() == 0) {
            _nextLocalId.store(registry->getMaxEntityId() + 1);
        }
        entity = _nextLocalId.fetch_add(1);
    } else {
        entity = registry->createEntity();
    }

    return entity;
}

}  // namespace ecs
