/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerInputProvider
*/

#include "ServerInputProvider.hpp"
#include <utility>

namespace ecs {

ServerInputProvider::ServerInputProvider() {
}

float ServerInputProvider::getAxisValue(event_t axis) {
    (void)axis;
    // TODO(anyone): Implement server-side input handling (network inputs)
    return 0.0f;
}

bool ServerInputProvider::isActionPressed(InputAction action) {
    (void)action;
    // TODO(anyone): Implement server-side input handling (network inputs)
    return false;
}

float ServerInputProvider::getActionAxis(InputAction action) {
    (void)action;
    // TODO(anyone): Implement server-side input handling (network inputs)
    return 0.0f;
}

}  // namespace ecs
