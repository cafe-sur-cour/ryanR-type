/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Colors
*/

#ifndef COLORS_HPP_
#define COLORS_HPP_

#include "../libs/Multimedia/IWindow.hpp"

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
    const gfx::color_t UI_BACKGROUND = {10, 10, 30};                            // Dark space blue
    const gfx::color_t UI_ACCENT = {0, 255, 255};                               // Cyan accent
    const gfx::color_t UI_TEXT = {255, 255, 255};                               // White text
    const gfx::color_t UI_TEXT_SECONDARY = {192, 192, 192};                     // Light gray
    const gfx::color_t UI_OUTLINE = {0, 0, 0};                                  // Black outline

    // Button Colors
    const gfx::color_t BUTTON_PRIMARY = {0, 100, 200};                          // Deep blue
    const gfx::color_t BUTTON_PRIMARY_HOVER = {0, 150, 255};                    // Bright blue
    const gfx::color_t BUTTON_PRIMARY_PRESSED = {0, 50, 150};                   // Dark blue
    const gfx::color_t BUTTON_SECONDARY = {100, 0, 150};                        // Purple
    const gfx::color_t BUTTON_SECONDARY_HOVER = {150, 0, 200};                  // Bright purple
    const gfx::color_t BUTTON_SECONDARY_PRESSED = {75, 0, 112};                 // Dark purple
    // Panel/Frame Colors
    const gfx::color_t PANEL_BACKGROUND = {20, 20, 50};                         // Slightly lighter than UI background
    const gfx::color_t PANEL_BORDER = {0, 200, 255};                            // Cyan border

    // Slider Colors
    const gfx::color_t SLIDER_TRACK = {50, 50, 100};                            // Dark blue track
    const gfx::color_t SLIDER_FILL = {0, 150, 255};                             // Bright blue fill
    const gfx::color_t SLIDER_HANDLE = {0, 100, 200};                           // Blue handle
    const gfx::color_t SLIDER_HANDLE_HOVER = {0, 150, 255};                     // Bright blue hover
    const gfx::color_t SLIDER_HANDLE_FOCUSED = {0, 200, 255};                   // Cyan focused
    const gfx::color_t SLIDER_LABEL = {255, 255, 255};                          // White label

    // Toggle Switch Colors
    const gfx::color_t TOGGLE_TRACK = {100, 50, 150};                           // Purple track
    const gfx::color_t TOGGLE_HANDLE = {150, 100, 200};                         // Light purple handle
    const gfx::color_t TOGGLE_HANDLE_HOVER = {200, 150, 255};                   // Bright purple hover
    const gfx::color_t TOGGLE_HANDLE_FOCUSED = {255, 200, 255};                 // Pinkish purple focused
    const gfx::color_t TOGGLE_ON = {150, 0, 200};                               // Purple on
    const gfx::color_t TOGGLE_OFF = {50, 0, 100};                               // Dark purple off

    // General UI States
    const gfx::color_t UI_HOVER = {0, 150, 255};                                // Bright blue hover
    const gfx::color_t UI_FOCUSED = {0, 200, 255};                              // Cyan focused
    const gfx::color_t UI_DISABLED = {100, 100, 100};                           // Gray disabled
}

#endif  // COLORS_HPP_
