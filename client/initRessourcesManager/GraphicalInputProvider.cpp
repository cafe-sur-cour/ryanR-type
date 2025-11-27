/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GraphicalInputProvider
*/

#include "GraphicalInputProvider.hpp"
#include <utility>

namespace ecs {

GraphicalInputProvider::GraphicalInputProvider(std::shared_ptr<gfx::IEvent> eventSystem)
    : _eventSystem(eventSystem) {
}

bool GraphicalInputProvider::isKeyPressed(event_t key) {
    return _eventSystem->isKeyPressed(key);
}

float GraphicalInputProvider::getAxisValue(event_t axis) {
    return _eventSystem->getAxisValue(axis);
}

std::pair<int, int> GraphicalInputProvider::getMousePos() {
    return _eventSystem->getMousePos();
}

bool GraphicalInputProvider::isMouseButtonPressed(int button) {
    return _eventSystem->isMouseButtonPressed(button);
}

}  // namespace ecs
