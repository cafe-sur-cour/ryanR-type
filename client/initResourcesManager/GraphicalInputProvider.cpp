/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GraphicalInputProvider
*/

#include "GraphicalInputProvider.hpp"
#include <utility>
#include <memory>

namespace ecs {

static bool isAxis(gfx::EventType et) {
    return et >= gfx::EventType::GAMEPAD_LEFT_TRIGGER
        && et <= gfx::EventType::GAMEPAD_RIGHT_STICK_RIGHT;
}

GraphicalInputProvider::GraphicalInputProvider(
    std::shared_ptr<gfx::IEvent> eventSystem,
    std::shared_ptr<InputMappingManager> mappingManager
)
    : _eventSystem(eventSystem), _mappingManager(mappingManager) {
}

float GraphicalInputProvider::getAxisValue(event_t axis, size_t clientID) {
    (void)clientID;
    return _eventSystem->getAxisValue(axis);
}

bool GraphicalInputProvider::isActionPressed(InputAction action, size_t clientID) {
    (void)clientID;
    const auto& mapping = _mappingManager->getMapping();
    auto it = mapping.mappings.find(action);
    if (it == mapping.mappings.end()) return false;
    for (auto& pair : it->second) {
        auto et = pair.first;
        if (_eventSystem->isKeyPressed(et)) return true;
    }
    return false;
}

float GraphicalInputProvider::getActionAxis(InputAction action, size_t clientID) {
    (void)clientID;
    const auto& mapping = _mappingManager->getMapping();
    auto it = mapping.mappings.find(action);
    if (it == mapping.mappings.end()) return 0.0f;
    float value = 0.0f;
    for (auto& pair : it->second) {
        auto et = pair.first;
        float contrib = pair.second;
        if (isAxis(et)) {
            value += _eventSystem->getAxisValue(et) * contrib;
        } else {
            if (_eventSystem->isKeyPressed(et)) {
                value += contrib;
            }
        }
    }
    return value;
}

InputMapping GraphicalInputProvider::getInputMapping(size_t clientID) const {
    (void)clientID;
    return _mappingManager->getMapping();
}

}  // namespace ecs
