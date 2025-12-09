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
#include "../constants.hpp"

namespace ecs {

InputMappingManager::InputMappingManager() {
    loadDefault();
}

void InputMappingManager::loadDefault() {
    /* MOVE_X */
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::LEFT] = -1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::RIGHT] = 1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::Q] = -1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::D] = 1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::GAMEPAD_DPAD_LEFT] = -1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::GAMEPAD_DPAD_RIGHT] = 1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::GAMEPAD_LEFT_STICK_LEFT] = -1.0f;
    _mapping.mappings[InputAction::MOVE_X][gfx::EventType::GAMEPAD_LEFT_STICK_RIGHT] = 1.0f;

    /* MOVE_Y */
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::UP] = -1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::DOWN] = 1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::Z] = -1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::S] = 1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::GAMEPAD_DPAD_UP] = -1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::GAMEPAD_DPAD_DOWN] = 1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::GAMEPAD_LEFT_STICK_UP] = -1.0f;
    _mapping.mappings[InputAction::MOVE_Y][gfx::EventType::GAMEPAD_LEFT_STICK_DOWN] = 1.0f;

    /* SHOOT */
    _mapping.mappings[InputAction::SHOOT][gfx::EventType::SPACE] = 1.0f;
    _mapping.mappings[InputAction::SHOOT][gfx::EventType::MOUSELEFTCLICK] = 1.0f;
    _mapping.mappings[InputAction::SHOOT][gfx::EventType::GAMEPAD_A] = 1.0f;

    /* PAUSE */
    _mapping.mappings[InputAction::PAUSE][gfx::EventType::ESCAPE] = 1.0f;
    _mapping.mappings[InputAction::PAUSE][gfx::EventType::GAMEPAD_START] = 1.0f;

    /* MENU_UP */
    _mapping.mappings[InputAction::MENU_UP][gfx::EventType::UP] = 1.0f;
    _mapping.mappings[InputAction::MENU_UP][gfx::EventType::Z] = 1.0f;
    _mapping.mappings[InputAction::MENU_UP][gfx::EventType::GAMEPAD_DPAD_UP] = 1.0f;

    /* MENU_DOWN */
    _mapping.mappings[InputAction::MENU_DOWN][gfx::EventType::DOWN] = 1.0f;
    _mapping.mappings[InputAction::MENU_DOWN][gfx::EventType::S] = 1.0f;
    _mapping.mappings[InputAction::MENU_DOWN][gfx::EventType::GAMEPAD_DPAD_DOWN] = 1.0f;

    /* MENU_LEFT */
    _mapping.mappings[InputAction::MENU_LEFT][gfx::EventType::LEFT] = 1.0f;
    _mapping.mappings[InputAction::MENU_LEFT][gfx::EventType::Q] = 1.0f;
    _mapping.mappings[InputAction::MENU_LEFT][gfx::EventType::GAMEPAD_DPAD_LEFT] = 1.0f;

    /* MENU_RIGHT */
    _mapping.mappings[InputAction::MENU_RIGHT][gfx::EventType::RIGHT] = 1.0f;
    _mapping.mappings[InputAction::MENU_RIGHT][gfx::EventType::D] = 1.0f;
    _mapping.mappings[InputAction::MENU_RIGHT][gfx::EventType::GAMEPAD_DPAD_RIGHT] = 1.0f;

    /* MENU_SELECT */
    _mapping.mappings[InputAction::MENU_SELECT][gfx::EventType::ENTER] = 1.0f;
    _mapping.mappings[InputAction::MENU_SELECT][gfx::EventType::GAMEPAD_A] = 1.0f;

    /* MENU_BACK */
    _mapping.mappings[InputAction::MENU_BACK][gfx::EventType::ESCAPE] = 1.0f;
    _mapping.mappings[InputAction::MENU_BACK][gfx::EventType::GAMEPAD_B] = 1.0f;
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

gfx::EventType InputMappingManager::getKeyboardKeyForAction(InputAction action) const {
    auto it = _mapping.mappings.find(action);
    if (it == _mapping.mappings.end())
        return gfx::EventType::NOTHING;

    for (const auto& pair : it->second) {
        if (isKeyboardKey(pair.first))
            return pair.first;
    }
    return gfx::EventType::NOTHING;
}

gfx::EventType InputMappingManager::getKeyboardKeyForActionDirection(
    InputAction action, float direction
) const {
    auto it = _mapping.mappings.find(action);
    if (it == _mapping.mappings.end())
        return gfx::EventType::NOTHING;

    for (const auto& pair : it->second) {
        if (isKeyboardKey(pair.first)) {
            if ((direction > 0 && pair.second > 0) || (direction < 0 && pair.second < 0))
                return pair.first;
        }
    }
    return gfx::EventType::NOTHING;
}

std::vector<gfx::EventType> InputMappingManager::getKeyboardKeysForAction(
    InputAction action
) const {
    std::vector<gfx::EventType> keys;
    auto it = _mapping.mappings.find(action);
    if (it == _mapping.mappings.end())
        return keys;

    for (const auto& pair : it->second) {
        if (isKeyboardKey(pair.first))
            keys.push_back(pair.first);
    }
    return keys;
}

void InputMappingManager::remapKeyboardKey(
    InputAction action, gfx::EventType oldKey, gfx::EventType newKey
) {
    auto it = _mapping.mappings.find(action);
    if (it == _mapping.mappings.end())
        return;

    auto keyIt = it->second.find(oldKey);
    if (keyIt == it->second.end())
        return;

    float value = keyIt->second;

    std::vector<gfx::EventType> keysToRemove;
    for (const auto& pair : it->second) {
        if (isKeyboardKey(pair.first) && std::fabs(pair.second - value) < constants::EPS) {
            keysToRemove.push_back(pair.first);
        }
    }
    for (const auto& key : keysToRemove) {
        it->second.erase(key);
    }

    it->second[newKey] = value;
}

bool InputMappingManager::isKeyboardKey(gfx::EventType eventType) {
    return eventType >= gfx::EventType::UP && eventType <= gfx::EventType::NUMPAD_ENTER;
}

std::string InputMappingManager::eventTypeToString(gfx::EventType eventType) {
    switch (eventType) {
        case gfx::EventType::UP: return "Up Arrow";
        case gfx::EventType::DOWN: return "Down Arrow";
        case gfx::EventType::LEFT: return "Left Arrow";
        case gfx::EventType::RIGHT: return "Right Arrow";
        case gfx::EventType::A: return "A";
        case gfx::EventType::B: return "B";
        case gfx::EventType::C: return "C";
        case gfx::EventType::D: return "D";
        case gfx::EventType::E: return "E";
        case gfx::EventType::F: return "F";
        case gfx::EventType::G: return "G";
        case gfx::EventType::H: return "H";
        case gfx::EventType::I: return "I";
        case gfx::EventType::J: return "J";
        case gfx::EventType::K: return "K";
        case gfx::EventType::L: return "L";
        case gfx::EventType::M: return "M";
        case gfx::EventType::N: return "N";
        case gfx::EventType::O: return "O";
        case gfx::EventType::P: return "P";
        case gfx::EventType::Q: return "Q";
        case gfx::EventType::R: return "R";
        case gfx::EventType::S: return "S";
        case gfx::EventType::T: return "T";
        case gfx::EventType::U: return "U";
        case gfx::EventType::V: return "V";
        case gfx::EventType::W: return "W";
        case gfx::EventType::X: return "X";
        case gfx::EventType::Y: return "Y";
        case gfx::EventType::Z: return "Z";
        case gfx::EventType::NUM0: return "0";
        case gfx::EventType::NUM1: return "1";
        case gfx::EventType::NUM2: return "2";
        case gfx::EventType::NUM3: return "3";
        case gfx::EventType::NUM4: return "4";
        case gfx::EventType::NUM5: return "5";
        case gfx::EventType::NUM6: return "6";
        case gfx::EventType::NUM7: return "7";
        case gfx::EventType::NUM8: return "8";
        case gfx::EventType::NUM9: return "9";
        case gfx::EventType::SPACE: return "Space";
        case gfx::EventType::ENTER: return "Enter";
        case gfx::EventType::ESCAPE: return "Escape";
        case gfx::EventType::TAB: return "Tab";
        case gfx::EventType::BACKSPACE: return "Backspace";
        case gfx::EventType::DELETE_KEY: return "Delete";
        case gfx::EventType::INSERT: return "Insert";
        case gfx::EventType::HOME: return "Home";
        case gfx::EventType::END: return "End";
        case gfx::EventType::PAGEUP: return "Page Up";
        case gfx::EventType::PAGEDOWN: return "Page Down";
        case gfx::EventType::F1: return "F1";
        case gfx::EventType::F2: return "F2";
        case gfx::EventType::F3: return "F3";
        case gfx::EventType::F4: return "F4";
        case gfx::EventType::F5: return "F5";
        case gfx::EventType::F6: return "F6";
        case gfx::EventType::F7: return "F7";
        case gfx::EventType::F8: return "F8";
        case gfx::EventType::F9: return "F9";
        case gfx::EventType::F10: return "F10";
        case gfx::EventType::F11: return "F11";
        case gfx::EventType::F12: return "F12";
        case gfx::EventType::LSHIFT: return "Left Shift";
        case gfx::EventType::RSHIFT: return "Right Shift";
        case gfx::EventType::LCTRL: return "Left Ctrl";
        case gfx::EventType::RCTRL: return "Right Ctrl";
        case gfx::EventType::LALT: return "Left Alt";
        case gfx::EventType::RALT: return "Right Alt";
        case gfx::EventType::NUMPAD0: return "Numpad 0";
        case gfx::EventType::NUMPAD1: return "Numpad 1";
        case gfx::EventType::NUMPAD2: return "Numpad 2";
        case gfx::EventType::NUMPAD3: return "Numpad 3";
        case gfx::EventType::NUMPAD4: return "Numpad 4";
        case gfx::EventType::NUMPAD5: return "Numpad 5";
        case gfx::EventType::NUMPAD6: return "Numpad 6";
        case gfx::EventType::NUMPAD7: return "Numpad 7";
        case gfx::EventType::NUMPAD8: return "Numpad 8";
        case gfx::EventType::NUMPAD9: return "Numpad 9";
        default: return "Unknown";
    }
}

}  // namespace ecs
