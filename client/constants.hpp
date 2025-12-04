/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Client Constants
*/

#ifndef CLIENT_CONSTANTS_HPP_
#define CLIENT_CONSTANTS_HPP_

#include "../common/constants.hpp"

namespace constants {
    /* Window */
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;

    /* UI */
    constexpr float INVALID_MOUSE_POSITION = -1.0f;
    constexpr float NAVIGATION_COOLDOWN_TIME = 0.2f;

    /* View */
    constexpr float VIEW_SMOOTHING_SPEED = 15.0f;

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
    };
}

#endif /* !CLIENT_CONSTANTS_HPP_ */
