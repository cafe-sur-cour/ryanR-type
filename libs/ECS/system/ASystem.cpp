/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ASystem
*/

#include "ASystem.hpp"

namespace ecs {

ASystem::ASystem() {
}

void ASystem::updateSystem(std::shared_ptr<AContext> context, float deltaTime) {
    if (context) {
        update(context, deltaTime);
    }
}

}  // namespace ecs
