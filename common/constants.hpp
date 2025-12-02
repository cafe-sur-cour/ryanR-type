/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Constants
*/

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstdint>

namespace constants {
    /* Network Defaults */
    constexpr int DEFAULT_SERVER_PORT = 4242;
    constexpr uint32_t DEFAULT_SERVER_IP = 0x7F000001;  // 127.0.0.1

    /* Game Defaults */
    constexpr float BASE_SPEED = 100.0f;
    constexpr float EPS = 1e-6f;
    constexpr float PLAYER_BASE_SPEED = 300.0f;
    constexpr float GAMEPAD_DEADZONE = 0.15f;
    constexpr int SMOOTH_MOVEMENT_ITERATIONS = 4;

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
    };

    /* Window */
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;
}

#endif /* !CONSTANTS_HPP_ */
