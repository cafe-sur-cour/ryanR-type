/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** KeyMappings
*/

#include <map>
#include "SfmlKeyMappings.hpp"

namespace gfx {

std::map<IEvent::event_t, sf::Keyboard::Key>
KeyMappings::createKeyboardMapping() {
    return {
        // Directional keys
        {EventType::UP, sf::Keyboard::Key::Up},
        {EventType::DOWN, sf::Keyboard::Key::Down},
        {EventType::LEFT, sf::Keyboard::Key::Left},
        {EventType::RIGHT, sf::Keyboard::Key::Right},

        // Alphanumeric keys
        {EventType::A, sf::Keyboard::Key::A},
        {EventType::B, sf::Keyboard::Key::B},
        {EventType::C, sf::Keyboard::Key::C},
        {EventType::D, sf::Keyboard::Key::D},
        {EventType::E, sf::Keyboard::Key::E},
        {EventType::F, sf::Keyboard::Key::F},
        {EventType::G, sf::Keyboard::Key::G},
        {EventType::H, sf::Keyboard::Key::H},
        {EventType::I, sf::Keyboard::Key::I},
        {EventType::J, sf::Keyboard::Key::J},
        {EventType::K, sf::Keyboard::Key::K},
        {EventType::L, sf::Keyboard::Key::L},
        {EventType::M, sf::Keyboard::Key::M},
        {EventType::N, sf::Keyboard::Key::N},
        {EventType::O, sf::Keyboard::Key::O},
        {EventType::P, sf::Keyboard::Key::P},
        {EventType::Q, sf::Keyboard::Key::Q},
        {EventType::R, sf::Keyboard::Key::R},
        {EventType::S, sf::Keyboard::Key::S},
        {EventType::T, sf::Keyboard::Key::T},
        {EventType::U, sf::Keyboard::Key::U},
        {EventType::V, sf::Keyboard::Key::V},
        {EventType::W, sf::Keyboard::Key::W},
        {EventType::X, sf::Keyboard::Key::X},
        {EventType::Y, sf::Keyboard::Key::Y},
        {EventType::Z, sf::Keyboard::Key::Z},
        {EventType::NUM0, sf::Keyboard::Key::Num0},
        {EventType::NUM1, sf::Keyboard::Key::Num1},
        {EventType::NUM2, sf::Keyboard::Key::Num2},
        {EventType::NUM3, sf::Keyboard::Key::Num3},
        {EventType::NUM4, sf::Keyboard::Key::Num4},
        {EventType::NUM5, sf::Keyboard::Key::Num5},
        {EventType::NUM6, sf::Keyboard::Key::Num6},
        {EventType::NUM7, sf::Keyboard::Key::Num7},
        {EventType::NUM8, sf::Keyboard::Key::Num8},
        {EventType::NUM9, sf::Keyboard::Key::Num9},

        // Special keys
        {EventType::SPACE, sf::Keyboard::Key::Space},
        {EventType::ENTER, sf::Keyboard::Key::Enter},
        {EventType::ESCAPE, sf::Keyboard::Key::Escape},
        {EventType::TAB, sf::Keyboard::Key::Tab},
        {EventType::BACKSPACE, sf::Keyboard::Key::Backspace},
        {EventType::DELETE_KEY, sf::Keyboard::Key::Delete},
        {EventType::INSERT, sf::Keyboard::Key::Insert},
        {EventType::HOME, sf::Keyboard::Key::Home},
        {EventType::END, sf::Keyboard::Key::End},
        {EventType::PAGEUP, sf::Keyboard::Key::PageUp},
        {EventType::PAGEDOWN, sf::Keyboard::Key::PageDown},

        // Function keys
        {EventType::F1, sf::Keyboard::Key::F1},
        {EventType::F2, sf::Keyboard::Key::F2},
        {EventType::F3, sf::Keyboard::Key::F3},
        {EventType::F4, sf::Keyboard::Key::F4},
        {EventType::F5, sf::Keyboard::Key::F5},
        {EventType::F6, sf::Keyboard::Key::F6},
        {EventType::F7, sf::Keyboard::Key::F7},
        {EventType::F8, sf::Keyboard::Key::F8},
        {EventType::F9, sf::Keyboard::Key::F9},
        {EventType::F10, sf::Keyboard::Key::F10},
        {EventType::F11, sf::Keyboard::Key::F11},
        {EventType::F12, sf::Keyboard::Key::F12},

        // Modifiers
        {EventType::LSHIFT, sf::Keyboard::Key::LShift},
        {EventType::RSHIFT, sf::Keyboard::Key::RShift},
        {EventType::LCTRL, sf::Keyboard::Key::LControl},
        {EventType::RCTRL, sf::Keyboard::Key::RControl},
        {EventType::LALT, sf::Keyboard::Key::LAlt},
        {EventType::RALT, sf::Keyboard::Key::RAlt},

        // Symbols
        {EventType::MINUS, sf::Keyboard::Key::Hyphen},
        {EventType::PLUS, sf::Keyboard::Key::Equal},
        {EventType::EQUALS, sf::Keyboard::Key::Equal},
        {EventType::LBRACKET, sf::Keyboard::Key::LBracket},
        {EventType::RBRACKET, sf::Keyboard::Key::RBracket},
        {EventType::SEMICOLON, sf::Keyboard::Key::Semicolon},
        {EventType::QUOTE, sf::Keyboard::Key::Apostrophe},
        {EventType::COMMA, sf::Keyboard::Key::Comma},
        {EventType::PERIOD, sf::Keyboard::Key::Period},
        {EventType::SLASH, sf::Keyboard::Key::Slash},
        {EventType::BACKSLASH, sf::Keyboard::Key::Backslash},
        {EventType::TILDE, sf::Keyboard::Key::Grave},

        // Numpad
        {EventType::NUMPAD0, sf::Keyboard::Key::Numpad0},
        {EventType::NUMPAD1, sf::Keyboard::Key::Numpad1},
        {EventType::NUMPAD2, sf::Keyboard::Key::Numpad2},
        {EventType::NUMPAD3, sf::Keyboard::Key::Numpad3},
        {EventType::NUMPAD4, sf::Keyboard::Key::Numpad4},
        {EventType::NUMPAD5, sf::Keyboard::Key::Numpad5},
        {EventType::NUMPAD6, sf::Keyboard::Key::Numpad6},
        {EventType::NUMPAD7, sf::Keyboard::Key::Numpad7},
        {EventType::NUMPAD8, sf::Keyboard::Key::Numpad8},
        {EventType::NUMPAD9, sf::Keyboard::Key::Numpad9},
        {EventType::NUMPAD_ADD, sf::Keyboard::Key::Add},
        {EventType::NUMPAD_SUBTRACT, sf::Keyboard::Key::Subtract},
        {EventType::NUMPAD_MULTIPLY, sf::Keyboard::Key::Multiply},
        {EventType::NUMPAD_DIVIDE, sf::Keyboard::Key::Divide},
        {EventType::NUMPAD_ENTER, sf::Keyboard::Key::Enter}
    };
}

std::map<IEvent::event_t, sf::Mouse::Button> KeyMappings::createMouseMapping() {
    return {
        {EventType::MOUSELEFTCLICK, sf::Mouse::Button::Left},
        {EventType::MOUSERIGHTCLICK, sf::Mouse::Button::Right},
        {EventType::MOUSEMIDDLECLICK, sf::Mouse::Button::Middle}
    };
}

std::map<IEvent::event_t, unsigned int>
KeyMappings::createJoystickButtonMapping() {
    return {
        {EventType::GAMEPAD_A, 0},              // A/Cross
        {EventType::GAMEPAD_B, 1},              // B/Circle
        {EventType::GAMEPAD_X, 2},              // X/Square
        {EventType::GAMEPAD_Y, 3},              // Y/Triangle
        {EventType::GAMEPAD_LEFT_BUMPER, 4},    // L1/LB
        {EventType::GAMEPAD_RIGHT_BUMPER, 5},   // R1/RB
        {EventType::GAMEPAD_BACK, 6},           // Back/Share
        {EventType::GAMEPAD_START, 7},          // Start/Options
        {EventType::GAMEPAD_GUIDE, 8},          // Guide/PS button
        {EventType::GAMEPAD_LEFT_THUMB, 9},     // Left stick click
        {EventType::GAMEPAD_RIGHT_THUMB, 10}    // Right stick click
    };
}
}  // namespace gfx
