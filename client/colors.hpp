/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Colors
*/

#ifndef COLORS_HPP_
#define COLORS_HPP_

#include "../common/interfaces/IWindow.hpp"

namespace colors {
    const gfx::color_t BLACK = {0, 0, 0};
    const gfx::color_t WHITE = {255, 255, 255};
    const gfx::color_t RED = {255, 0, 0};
    const gfx::color_t GREEN = {0, 255, 0};
    const gfx::color_t BLUE = {0, 0, 255};
    const gfx::color_t YELLOW = {255, 255, 0};
    const gfx::color_t CYAN = {0, 255, 255};
    const gfx::color_t MAGENTA = {255, 0, 255};
    const gfx::color_t GRAY = {128, 128, 128};
    const gfx::color_t LIGHT_GRAY = {192, 192, 192};
    const gfx::color_t DARK_GRAY = {64, 64, 64};
    const gfx::color_t ORANGE = {255, 165, 0};
    const gfx::color_t PURPLE = {128, 0, 128};
    const gfx::color_t PINK = {255, 192, 203};
    const gfx::color_t BROWN = {165, 42, 42};

    // UI Colors
    const gfx::color_t UI_BACKGROUND = {0, 0, 0};                               // Black background
    const gfx::color_t UI_ACCENT = {253, 208, 16};                              // Yellow accent
    const gfx::color_t UI_TEXT = {255, 255, 255};                               // White text
    const gfx::color_t UI_TEXT_SECONDARY = {253, 208, 16};                      // Yellow secondary text
    const gfx::color_t UI_OUTLINE = {0, 0, 0};                                  // Black outline

    // Button Colors
    const gfx::color_t BUTTON_PRIMARY = {224,51,14};                            // Red primary button
    const gfx::color_t BUTTON_PRIMARY_HOVER = {245, 81, 47};                    // Bright red hover
    const gfx::color_t BUTTON_PRIMARY_PRESSED = {255, 48, 0};                   // Dark red pressed
    const gfx::color_t BUTTON_SECONDARY = {240, 207, 65};                       // Yellow secondary button
    const gfx::color_t BUTTON_SECONDARY_HOVER = {253, 208, 16};                 // Bright yellow hover
    const gfx::color_t BUTTON_SECONDARY_PRESSED = {252, 205, 3};                // Dark yellow pressed
    const gfx::color_t BUTTON_DANGER = {222, 77, 47};                           // Red danger
    const gfx::color_t BUTTON_DANGER_HOVER = {224,51,14};                       // Bright red hover
    const gfx::color_t BUTTON_DANGER_PRESSED = {247, 47, 5};                    // Dark red pressed

    // Panel/Frame Colors
    const gfx::color_t PANEL_BACKGROUND = {20, 20, 50};                         // Slightly lighter than UI background
    const gfx::color_t PANEL_BORDER = {0, 200, 255};                            // Cyan border

    // Slider Colors
    const gfx::color_t SLIDER_TRACK = {50, 50, 50};                             // Dark gray track
    const gfx::color_t SLIDER_FILL = {253, 208, 16};                            // Yellow fill
    const gfx::color_t SLIDER_HANDLE = {240, 207, 65};                          // Light yellow handle
    const gfx::color_t SLIDER_HANDLE_HOVER = {253, 208, 16};                    // Bright yellow hover
    const gfx::color_t SLIDER_HANDLE_FOCUSED = {252, 205, 3};                   // Dark yellow focused
    const gfx::color_t SLIDER_LABEL = {255, 255, 255};                          // White label

    // Toggle Switch Colors
    const gfx::color_t TOGGLE_TRACK = {100, 100, 100};                          // Gray track
    const gfx::color_t TOGGLE_HANDLE = {224, 51, 14};                           // Red handle
    const gfx::color_t TOGGLE_HANDLE_HOVER = {245, 81, 47};                     // Bright red hover
    const gfx::color_t TOGGLE_HANDLE_FOCUSED = {255, 48, 0};                    // Dark red focused
    const gfx::color_t TOGGLE_ON = {253, 208, 16};                              // Yellow on
    const gfx::color_t TOGGLE_OFF = {100, 100, 100};                            // Gray off

    const gfx::color_t UI_HOVER = {0, 150, 255};                                // Cyan hover
    const gfx::color_t UI_FOCUSED = {175, 175, 175};                            // Light gray focused
    const gfx::color_t UI_DISABLED = {100, 100, 100};                           // Gray disabled

    const gfx::color_t LEVEL_EDITOR_PANEL_BACKGROUND = {15, 10, 25};            // Dark with slight purple tint
    const gfx::color_t LEVEL_EDITOR_PANEL_BORDER = {200, 200, 200};             // Light gray border

    /* Player colors */
    const gfx::color_t PLAYER_LOCAL = {255, 255, 255, 255};                     // White for local player
    const gfx::color_t PLAYER_REMOTE = {150, 150, 150, 200};                    // Grayish for remote players
}

#endif  // COLORS_HPP_
