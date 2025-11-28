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

bool ServerInputProvider::isKeyPressed(event_t key) {
    (void)key;
    // TODO(anyone): Implement server-side input handling (network inputs)
    return false;
}

float ServerInputProvider::getAxisValue(event_t axis) {
    (void)axis;
    // TODO(anyone): Implement server-side input handling (network inputs)
    return 0.0f;
}

std::pair<int, int> ServerInputProvider::getMousePos() {
    // TODO(anyone): Implement server-side input handling (network inputs)
    return {0, 0};
}

bool ServerInputProvider::isMouseButtonPressed(int button) {
    (void)button;
    // TODO(anyone): Implement server-side input handling (network inputs)
    return false;
}

}  // namespace ecs
