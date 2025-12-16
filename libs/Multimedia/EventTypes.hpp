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
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,

    // Alphanumeric keys
    A = 4, B = 5, C = 6, D = 7, E = 8, F = 9, G = 10, H = 11, I = 12, J = 13, K = 14, L = 15, M = 16,
    N = 17, O = 18, P = 19, Q = 20, R = 21, S = 22, T = 23, U = 24, V = 25, W = 26, X = 27, Y = 28, Z = 29,
    NUM0 = 30, NUM1 = 31, NUM2 = 32, NUM3 = 33, NUM4 = 34, NUM5 = 35, NUM6 = 36, NUM7 = 37, NUM8 = 38, NUM9 = 39,

    // Special keys
    SPACE = 40,
    ENTER = 41,
    ESCAPE = 42,
    TAB = 43,
    BACKSPACE = 44,
    DELETE_KEY = 45,
    INSERT = 46,
    HOME = 47,
    END = 48,
    PAGEUP = 49,
    PAGEDOWN = 50,

    // Function keys
    F1 = 51, F2 = 52, F3 = 53, F4 = 54, F5 = 55, F6 = 56, F7 = 57, F8 = 58, F9 = 59, F10 = 60, F11 = 61, F12 = 62,

    // Modifiers
    LSHIFT = 63,
    RSHIFT = 64,
    LCTRL = 65,
    RCTRL = 66,
    LALT = 67,
    RALT = 68,

    // Symbols
    MINUS = 69,
    PLUS = 70,
    EQUALS = 71,
    LBRACKET = 72,
    RBRACKET = 73,
    SEMICOLON = 74,
    QUOTE = 75,
    COMMA = 76,
    PERIOD = 77,
    SLASH = 78,
    BACKSLASH = 79,
    TILDE = 80,

    // Numpad
    NUMPAD0 = 81, NUMPAD1 = 82, NUMPAD2 = 83, NUMPAD3 = 84, NUMPAD4 = 85,
    NUMPAD5 = 86, NUMPAD6 = 87, NUMPAD7 = 88, NUMPAD8 = 89, NUMPAD9 = 90,
    NUMPAD_ADD = 91,
    NUMPAD_SUBTRACT = 92,
    NUMPAD_MULTIPLY = 93,
    NUMPAD_DIVIDE = 94,
    NUMPAD_ENTER = 95,

    // Mouse buttons
    MOUSECLICK = 96,
    MOUSERIGHTCLICK = 97,
    MOUSELEFTCLICK = 98,
    MOUSEMIDDLECLICK = 99,
    MOUSELEFTRELEASE = 100,
    MOUSERIGHTRELEASE = 101,
    MOUSEMIDDLERELEASE = 102,
    MOUSEX1 = 103,
    MOUSEX2 = 104,
    MOUSEWHEELUP = 105,
    MOUSEWHEELDOWN = 106,

    // Gamepad/Controller buttons
    GAMEPAD_A = 107,
    GAMEPAD_B = 108,
    GAMEPAD_X = 109,
    GAMEPAD_Y = 110,
    GAMEPAD_LEFT_BUMPER = 111,
    GAMEPAD_RIGHT_BUMPER = 112,
    GAMEPAD_BACK = 113,
    GAMEPAD_START = 114,
    GAMEPAD_GUIDE = 115,
    GAMEPAD_LEFT_THUMB = 116,
    GAMEPAD_RIGHT_THUMB = 117,

    // Gamepad D-Pad
    GAMEPAD_DPAD_UP = 118,
    GAMEPAD_DPAD_DOWN = 119,
    GAMEPAD_DPAD_LEFT = 120,
    GAMEPAD_DPAD_RIGHT = 121,

    // Gamepad triggers
    GAMEPAD_LEFT_TRIGGER = 122,
    GAMEPAD_RIGHT_TRIGGER = 123,

    // Gamepad analog sticks
    GAMEPAD_LEFT_STICK_UP = 124,
    GAMEPAD_LEFT_STICK_DOWN = 125,
    GAMEPAD_LEFT_STICK_LEFT = 126,
    GAMEPAD_LEFT_STICK_RIGHT = 127,
    GAMEPAD_RIGHT_STICK_UP = 128,
    GAMEPAD_RIGHT_STICK_DOWN = 129,
    GAMEPAD_RIGHT_STICK_LEFT = 130,
    GAMEPAD_RIGHT_STICK_RIGHT = 131,

    // Window events
    CLOSE = 132,
    REFRESH = 133,

    // Text input
    TEXT_INPUT = 134,

    // Default
    NOTHING = 135
};

} // namespace gfx

#endif /* !EVENTTYPES_HPP_ */
