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

    bool isMenuAction = (action == InputAction::MENU_UP ||
                        action == InputAction::MENU_DOWN ||
                        action == InputAction::MENU_LEFT ||
                        action == InputAction::MENU_RIGHT ||
                        action == InputAction::MENU_SELECT ||
                        action == InputAction::MENU_BACK);

    if (_toggleMode && !isMenuAction) {
        bool keyPressed = false;
        for (auto& pair : it->second) {
            auto et = pair.first;
            if (_eventSystem->isKeyPressed(et)) {
                keyPressed = true;
                break;
            }
        }

        bool wasPressed = _lastKeyState[action];
        _lastKeyState[action] = keyPressed;

        if (keyPressed && !wasPressed) {
            _toggledStates[action] = !_toggledStates[action];
        }

        return _toggledStates[action];
    } else {
        for (auto& pair : it->second) {
            auto et = pair.first;
            if (_eventSystem->isKeyPressed(et)) return true;
        }
        return false;
    }
}

float GraphicalInputProvider::getActionAxis(InputAction action, size_t clientID) {
    (void)clientID;
    const auto& mapping = _mappingManager->getMapping();
    auto it = mapping.mappings.find(action);
    if (it == mapping.mappings.end()) return 0.0f;

    bool isMenuAction = (action == InputAction::MENU_UP ||
                        action == InputAction::MENU_DOWN ||
                        action == InputAction::MENU_LEFT ||
                        action == InputAction::MENU_RIGHT ||
                        action == InputAction::MENU_SELECT ||
                        action == InputAction::MENU_BACK);

    float value = 0.0f;
    for (auto& pair : it->second) {
        auto et = pair.first;
        float contrib = pair.second;
        if (isAxis(et)) {
            value += _eventSystem->getAxisValue(et) * contrib;
        } else {
            bool keyPressed = false;

            if (_toggleMode && !isMenuAction) {
                bool isPressedNow = _eventSystem->isKeyPressed(et);

                auto key = std::make_pair(action, et);
                bool wasPressedBefore = _keyPressedState[key];
                _keyPressedState[key] = isPressedNow;

                if (isPressedNow && !wasPressedBefore) {
                    _toggledKeyStates[key] = !_toggledKeyStates[key];

                    if (_toggledKeyStates[key]) {
                        for (auto& otherPair : it->second) {
                            auto otherEt = otherPair.first;
                            float otherContrib = otherPair.second;

                            if ((contrib > 0 && otherContrib < 0) ||
                                (contrib < 0 && otherContrib > 0)) {
                                auto otherKey = std::make_pair(action, otherEt);
                                _toggledKeyStates[otherKey] = false;
                            }
                        }
                    }
                }

                keyPressed = _toggledKeyStates[key];
            } else {
                keyPressed = _eventSystem->isKeyPressed(et);
            }

            if (keyPressed) {
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

void GraphicalInputProvider::setToggleMode(bool enabled) {
    _toggleMode = enabled;
    _toggledStates.clear();
    _lastKeyState.clear();
    _keyPressedState.clear();
    _toggledKeyStates.clear();
}

bool GraphicalInputProvider::isToggleMode() const {
    return _toggleMode;
}

}  // namespace ecs
