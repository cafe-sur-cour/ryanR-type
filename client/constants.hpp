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
    /* Timeout */
    constexpr int NETWORK_TIMEOUT = 5;

    /* Window */
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;

    /* UI */
    constexpr float INVALID_MOUSE_POSITION = -1.0f;
    constexpr float NAVIGATION_COOLDOWN_TIME = 0.2f;
    constexpr float UI_SCALE_SMALL = 0.75f;
    constexpr float UI_SCALE_NORMAL = 1.0f;
    constexpr float UI_SCALE_LARGE = 1.25f;
    constexpr size_t BUTTON_FONT_SIZE_BASE = 24;

    /* View */
    constexpr float VIEW_SMOOTHING_SPEED = 15.0f;

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
    };

    /* Accessibility Filters */
    const std::string FILTER_HIGH_CONTRAST_SHADER_PATH = "assets/shaders/highcontrast.frag";
    const std::string FILTER_PROTANOPIA_SHADER_PATH = "assets/shaders/protanopia.frag";
    const std::string FILTER_DEUTERANOPIA_SHADER_PATH = "assets/shaders/deuteranopia.frag";
    const std::string FILTER_TRITANOPIA_SHADER_PATH = "assets/shaders/tritanopia.frag";
    const std::string FILTER_BRIGHTNESS_SHADER_PATH = "assets/shaders/brightness.frag";
}

#endif /* !CLIENT_CONSTANTS_HPP_ */
