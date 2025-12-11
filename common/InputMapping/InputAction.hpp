/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InputAction
*/

#ifndef INPUTACTION_HPP_
#define INPUTACTION_HPP_

namespace ecs {

enum class InputAction {
    MOVE_X,
    MOVE_Y,
    SHOOT,
    PAUSE,
    MENU_UP,
    MENU_DOWN,
    MENU_LEFT,
    MENU_RIGHT,
    MENU_SELECT,
    MENU_BACK,
};

enum class RemappableAction {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    SHOOT,
};

}  // namespace ecs

#endif /* !INPUTACTION_HPP_ */
