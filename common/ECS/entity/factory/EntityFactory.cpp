/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EntityFactory
*/

#include "EntityFactory.hpp"
#include <memory>
#include "../../../components/permanent/NetworkIdComponent.hpp"

namespace ecs {

EntityFactory::EntityFactory(size_t startingNetworkId)
    : _nextNetworkId(startingNetworkId), _nextLocalId(0) {}

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

    if (context.shouldHaveNetworkId()) {
        size_t networkId = resolveNetworkId(context);
        auto networkIdComponent = std::make_shared<NetworkIdComponent>(networkId);
        registry->addComponent<NetworkIdComponent>(entity, networkIdComponent);
    }

    return entity;
}

size_t EntityFactory::getNextNetworkId() const {
    return _nextNetworkId.load();
}

void EntityFactory::setNextNetworkId(size_t nextId) {
    _nextNetworkId.store(nextId);
}

size_t EntityFactory::resolveNetworkId(const EntityCreationContext& context) {
    if (context.networkId.has_value()) {
        return context.networkId.value();
    }

    if (context.origin == EntityCreationOrigin::SERVER) {
        return _nextNetworkId.fetch_add(1);
    }

    return 0;
}

}  // namespace ecs
