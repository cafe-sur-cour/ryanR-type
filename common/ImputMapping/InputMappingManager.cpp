/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputMappingManager
*/

#include "InputMappingManager.hpp"

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

}  // namespace ecs
