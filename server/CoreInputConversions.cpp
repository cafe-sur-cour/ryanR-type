/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Core Input Conversions
*/

#include "Core.hpp"
#include "../common/constants.hpp"
#include "../common/InputMapping/InputAction.hpp"

ecs::InputAction Core::getInputActionFromEvent(constants::EventType eventType) const {
    if (eventType == constants::EventType::UP || eventType == constants::EventType::DOWN) {
        return ecs::InputAction::MOVE_Y;
    }
    if (eventType == constants::EventType::LEFT || eventType == constants::EventType::RIGHT) {
        return ecs::InputAction::MOVE_X;
    }
    return ecs::InputAction::SHOOT;
}
