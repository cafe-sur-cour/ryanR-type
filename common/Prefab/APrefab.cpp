/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** APrefab
*/

#include "APrefab.hpp"
#include <memory>

size_t APrefab::instantiate(const std::shared_ptr<ecs::ARegistry> &registry) {
    return registry->createEntity();
}
