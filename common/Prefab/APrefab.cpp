/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** APrefab
*/

#include "APrefab.hpp"
#include <memory>

ecs::Entity APrefab::instantiate(
    const std::shared_ptr<ecs::Registry>& registry,
    const std::shared_ptr<ecs::IEntityFactory>& factory,
    const ecs::EntityCreationContext& context
) {
    return factory->createEntity(registry, context);
}

ecs::Entity APrefab::instantiate(const std::shared_ptr<ecs::Registry>& registry) {
    return registry->createEntity();
}
