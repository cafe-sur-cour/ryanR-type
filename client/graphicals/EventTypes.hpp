/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** EventTypes
*/

#ifndef EVENTTYPES_HPP_
#define EVENTTYPES_HPP_

namespace gfx {

enum class EventType {
    // Directional keys
    UP,
    DOWN,
    LEFT,
    RIGHT,

    // Alphanumeric keys
    A, B, C, D, E, F, G, H, I, J, K, L, M,
    N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
    NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,

    // Special keys
    SPACE,
    ENTER,
    ESCAPE,
    TAB,
    BACKSPACE,
    DELETE_KEY,
    INSERT,
    HOME,
    END,
    PAGEUP,
    PAGEDOWN,

    // Function keys
    F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

    // Modifiers
    LSHIFT,
    RSHIFT,
    LCTRL,
    RCTRL,
    LALT,
    RALT,

    // Symbols
    MINUS,
    PLUS,
    EQUALS,
    LBRACKET,
    RBRACKET,
    SEMICOLON,
    QUOTE,
    COMMA,
    PERIOD,
    SLASH,
    BACKSLASH,
    TILDE,

    // Numpad
    NUMPAD0, NUMPAD1, NUMPAD2, NUMPAD3, NUMPAD4,
    NUMPAD5, NUMPAD6, NUMPAD7, NUMPAD8, NUMPAD9,
    NUMPAD_ADD,
    NUMPAD_SUBTRACT,
    NUMPAD_MULTIPLY,
    NUMPAD_DIVIDE,
    NUMPAD_ENTER,

    // Mouse buttons
    MOUSECLICK,
    MOUSERIGHTCLICK,
    MOUSELEFTCLICK,
    MOUSEMIDDLECLICK,
    MOUSEX1,
    MOUSEX2,
    MOUSEWHEELUP,
    MOUSEWHEELDOWN,

    // Gamepad/Controller buttons
    GAMEPAD_A,
    GAMEPAD_B,
    GAMEPAD_X,
    GAMEPAD_Y,
    GAMEPAD_LEFT_BUMPER,
    GAMEPAD_RIGHT_BUMPER,
    GAMEPAD_BACK,
    GAMEPAD_START,
    GAMEPAD_GUIDE,
    GAMEPAD_LEFT_THUMB,
    GAMEPAD_RIGHT_THUMB,

    // Gamepad D-Pad
    GAMEPAD_DPAD_UP,
    GAMEPAD_DPAD_DOWN,
    GAMEPAD_DPAD_LEFT,
    GAMEPAD_DPAD_RIGHT,

    // Gamepad triggers
    GAMEPAD_LEFT_TRIGGER,
    GAMEPAD_RIGHT_TRIGGER,

    // Gamepad analog sticks
    GAMEPAD_LEFT_STICK_UP,
    GAMEPAD_LEFT_STICK_DOWN,
    GAMEPAD_LEFT_STICK_LEFT,
    GAMEPAD_LEFT_STICK_RIGHT,
    GAMEPAD_RIGHT_STICK_UP,
    GAMEPAD_RIGHT_STICK_DOWN,
    GAMEPAD_RIGHT_STICK_LEFT,
    GAMEPAD_RIGHT_STICK_RIGHT,

    // Window events
    CLOSE,
    REFRESH,

    // Default
    NOTHING
};

} // namespace gfx

#endif /* !EVENTTYPES_HPP_ */