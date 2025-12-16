/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputMappingManager
*/

#include "InputMappingManager.hpp"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <map>
#include "../constants.hpp"

namespace ecs {

const std::map<std::string, gfx::EventType> stringToEventMap = {
    {"Up", gfx::EventType::UP},
    {"Down", gfx::EventType::DOWN},
    {"Left", gfx::EventType::LEFT},
    {"Right", gfx::EventType::RIGHT},
    {"A", gfx::EventType::A},
    {"B", gfx::EventType::B},
    {"C", gfx::EventType::C},
    {"D", gfx::EventType::D},
    {"E", gfx::EventType::E},
    {"F", gfx::EventType::F},
    {"G", gfx::EventType::G},
    {"H", gfx::EventType::H},
    {"I", gfx::EventType::I},
    {"J", gfx::EventType::J},
    {"K", gfx::EventType::K},
    {"L", gfx::EventType::L},
    {"M", gfx::EventType::M},
    {"N", gfx::EventType::N},
    {"O", gfx::EventType::O},
    {"P", gfx::EventType::P},
    {"Q", gfx::EventType::Q},
    {"R", gfx::EventType::R},
    {"S", gfx::EventType::S},
    {"T", gfx::EventType::T},
    {"U", gfx::EventType::U},
    {"V", gfx::EventType::V},
    {"W", gfx::EventType::W},
    {"X", gfx::EventType::X},
    {"Y", gfx::EventType::Y},
    {"Z", gfx::EventType::Z},
    {"0", gfx::EventType::NUM0},
    {"1", gfx::EventType::NUM1},
    {"2", gfx::EventType::NUM2},
    {"3", gfx::EventType::NUM3},
    {"4", gfx::EventType::NUM4},
    {"5", gfx::EventType::NUM5},
    {"6", gfx::EventType::NUM6},
    {"7", gfx::EventType::NUM7},
    {"8", gfx::EventType::NUM8},
    {"9", gfx::EventType::NUM9},
    {"Space", gfx::EventType::SPACE},
    {"Enter", gfx::EventType::ENTER},
    {"Escape", gfx::EventType::ESCAPE},
    {"Tab", gfx::EventType::TAB},
    {"Backspace", gfx::EventType::BACKSPACE},
    {"Delete", gfx::EventType::DELETE_KEY},
    {"Insert", gfx::EventType::INSERT},
    {"Home", gfx::EventType::HOME},
    {"End", gfx::EventType::END},
    {"Page Up", gfx::EventType::PAGEUP},
    {"Page Down", gfx::EventType::PAGEDOWN},
    {"F1", gfx::EventType::F1},
    {"F2", gfx::EventType::F2},
    {"F3", gfx::EventType::F3},
    {"F4", gfx::EventType::F4},
    {"F5", gfx::EventType::F5},
    {"F6", gfx::EventType::F6},
    {"F7", gfx::EventType::F7},
    {"F8", gfx::EventType::F8},
    {"F9", gfx::EventType::F9},
    {"F10", gfx::EventType::F10},
    {"F11", gfx::EventType::F11},
    {"F12", gfx::EventType::F12},
    {"Left Shift", gfx::EventType::LSHIFT},
    {"Right Shift", gfx::EventType::RSHIFT},
    {"Left Ctrl", gfx::EventType::LCTRL},
    {"Right Ctrl", gfx::EventType::RCTRL},
    {"Left Alt", gfx::EventType::LALT},
    {"Right Alt", gfx::EventType::RALT},
    {"Numpad 0", gfx::EventType::NUMPAD0},
    {"Numpad 1", gfx::EventType::NUMPAD1},
    {"Numpad 2", gfx::EventType::NUMPAD2},
    {"Numpad 3", gfx::EventType::NUMPAD3},
    {"Numpad 4", gfx::EventType::NUMPAD4},
    {"Numpad 5", gfx::EventType::NUMPAD5},
    {"Numpad 6", gfx::EventType::NUMPAD6},
    {"Numpad 7", gfx::EventType::NUMPAD7},
    {"Numpad 8", gfx::EventType::NUMPAD8},
    {"Numpad 9", gfx::EventType::NUMPAD9}
};

const std::map<std::string, RemappableAction> stringToActionMap = {
    {"MOVE_LEFT", RemappableAction::MOVE_LEFT},
    {"MOVE_RIGHT", RemappableAction::MOVE_RIGHT},
    {"MOVE_UP", RemappableAction::MOVE_UP},
    {"MOVE_DOWN", RemappableAction::MOVE_DOWN},
    {"SHOOT", RemappableAction::SHOOT}
};

InputMappingManager::InputMappingManager() {
    loadDefault();
}

void InputMappingManager::loadDefault() {
    _mapping.remappableKeys[RemappableAction::MOVE_LEFT] = RemappableKeyBinding(
        gfx::EventType::Q, gfx::EventType::LEFT
    );
    _mapping.remappableKeys[RemappableAction::MOVE_RIGHT] = RemappableKeyBinding(
        gfx::EventType::D, gfx::EventType::RIGHT
    );
    _mapping.remappableKeys[RemappableAction::MOVE_UP] = RemappableKeyBinding(
        gfx::EventType::Z, gfx::EventType::UP
    );
    _mapping.remappableKeys[RemappableAction::MOVE_DOWN] = RemappableKeyBinding(
        gfx::EventType::S, gfx::EventType::DOWN
    );
    _mapping.remappableKeys[RemappableAction::SHOOT] = RemappableKeyBinding(
        gfx::EventType::SPACE, gfx::EventType::NOTHING
    );

    _mapping.fixedMappings[InputAction::MOVE_X]
        [gfx::EventType::GAMEPAD_DPAD_LEFT] = -1.0f;
    _mapping.fixedMappings[InputAction::MOVE_X]
        [gfx::EventType::GAMEPAD_DPAD_RIGHT] = 1.0f;
    _mapping.fixedMappings[InputAction::MOVE_X]
        [gfx::EventType::GAMEPAD_LEFT_STICK_LEFT] = -1.0f;
    _mapping.fixedMappings[InputAction::MOVE_X]
        [gfx::EventType::GAMEPAD_LEFT_STICK_RIGHT] = 1.0f;

    _mapping.fixedMappings[InputAction::MOVE_Y]
        [gfx::EventType::GAMEPAD_DPAD_UP] = -1.0f;
    _mapping.fixedMappings[InputAction::MOVE_Y]
        [gfx::EventType::GAMEPAD_DPAD_DOWN] = 1.0f;
    _mapping.fixedMappings[InputAction::MOVE_Y]
        [gfx::EventType::GAMEPAD_LEFT_STICK_UP] = -1.0f;
    _mapping.fixedMappings[InputAction::MOVE_Y]
        [gfx::EventType::GAMEPAD_LEFT_STICK_DOWN] = 1.0f;

    _mapping.fixedMappings[InputAction::SHOOT][gfx::EventType::GAMEPAD_A] = 1.0f;

    _mapping.fixedMappings[InputAction::PAUSE][gfx::EventType::ESCAPE] = 1.0f;
    _mapping.fixedMappings[InputAction::PAUSE][gfx::EventType::GAMEPAD_START] = 1.0f;

    _mapping.fixedMappings[InputAction::MENU_UP][gfx::EventType::UP] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_UP][gfx::EventType::Z] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_UP][gfx::EventType::GAMEPAD_DPAD_UP] = 1.0f;

    _mapping.fixedMappings[InputAction::MENU_DOWN][gfx::EventType::DOWN] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_DOWN][gfx::EventType::S] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_DOWN][gfx::EventType::GAMEPAD_DPAD_DOWN] = 1.0f;

    _mapping.fixedMappings[InputAction::MENU_LEFT][gfx::EventType::LEFT] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_LEFT][gfx::EventType::Q] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_LEFT][gfx::EventType::GAMEPAD_DPAD_LEFT] = 1.0f;

    _mapping.fixedMappings[InputAction::MENU_RIGHT][gfx::EventType::RIGHT] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_RIGHT][gfx::EventType::D] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_RIGHT][gfx::EventType::GAMEPAD_DPAD_RIGHT] = 1.0f;

    _mapping.fixedMappings[InputAction::MENU_SELECT][gfx::EventType::ENTER] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_SELECT][gfx::EventType::GAMEPAD_A] = 1.0f;

    _mapping.fixedMappings[InputAction::MENU_BACK][gfx::EventType::ESCAPE] = 1.0f;
    _mapping.fixedMappings[InputAction::MENU_BACK][gfx::EventType::GAMEPAD_B] = 1.0f;
}

void InputMappingManager::setMapping(const InputMapping& mapping) {
    _mapping = mapping;
}

const InputMapping& InputMappingManager::getMapping() const {
    return _mapping;
}

InputMapping& InputMappingManager::getMutableMapping() {
    return _mapping;
}

gfx::EventType InputMappingManager::getKeyForRemappableAction(
    RemappableAction action,
    bool getPrimary
) const {
    auto it = _mapping.remappableKeys.find(action);
    if (it != _mapping.remappableKeys.end()) {
        return getPrimary ? it->second.primary : it->second.secondary;
    }
    return gfx::EventType::NOTHING;
}

void InputMappingManager::remapKey(
    RemappableAction action,
    gfx::EventType newKey,
    bool setPrimary
) {
    auto it = _mapping.remappableKeys.find(action);
    if (it != _mapping.remappableKeys.end()) {
        if (setPrimary) {
            it->second.primary = newKey;
        } else {
            it->second.secondary = newKey;
        }
    }
}

bool InputMappingManager::isKeyboardKey(gfx::EventType eventType) {
    return eventType >= gfx::EventType::UP && eventType <= gfx::EventType::NUMPAD_ENTER;
}

std::string InputMappingManager::eventTypeToString(gfx::EventType eventType) {
    for (const auto& pair : stringToEventMap) {
        if (pair.second == eventType) {
            return pair.first;
        }
    }
    return "Unknown";
}

gfx::EventType InputMappingManager::stringToEventType(const std::string& str) {
    auto it = stringToEventMap.find(str);
    return it != stringToEventMap.end() ? it->second : gfx::EventType::NOTHING;
}

std::string InputMappingManager::remappableActionToString(RemappableAction action) {
    for (const auto& pair : stringToActionMap) {
        if (pair.second == action) {
            return pair.first;
        }
    }
    return "UNKNOWN";
}

RemappableAction InputMappingManager::stringToRemappableAction(const std::string& str) {
    auto it = stringToActionMap.find(str);
    return it != stringToActionMap.end() ? it->second : RemappableAction::SHOOT;
}

}  // namespace ecs
