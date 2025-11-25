/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlEvent
*/

#include <utility>
#include <string>
#include <memory>
#include "SfmlWindow.hpp"
#include "SfmlEvent.hpp"

using event_t = gfx::IEvent::event_t;

SfmlEvent::SfmlEvent(std::shared_ptr<ecs::ResourceManager> resourceManager,
    std::shared_ptr<gfx::IWindow> window)
    : _resourceManager(resourceManager), _window(window) {
    init();
}

SfmlEvent::~SfmlEvent() {}

void SfmlEvent::init() {
    initializeMappings();
    createReverseKeyboardMapping();
}

void SfmlEvent::initializeMappings() {
    _keyMap = gfx::KeyMappings::createKeyboardMapping();
    _mouseMap = gfx::KeyMappings::createMouseMapping();
    _joystickButtonMap = gfx::KeyMappings::createJoystickButtonMapping();
}

void SfmlEvent::createReverseKeyboardMapping() {
    for (const auto& pair : _keyMap) {
        _reverseKeyMap[pair.second] = pair.first;
    }
}

gfx::IEvent::event_t SfmlEvent::pollEvents() {
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(_window);
    if (!sfmlWindow)
        return event_t::NOTHING;
    auto window = sfmlWindow->getSfmlWindow();

    while (auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            return event_t::CLOSE;
        } else if (const auto* keyPressed =
                event->getIf<sf::Event::KeyPressed>()) {
            return processKeyboardEvent(*keyPressed);
        } else if (const auto* mousePressed =
                event->getIf<sf::Event::MouseButtonPressed>()) {
            return processMouseEvent(*mousePressed);
        } else if (const auto* joystickPressed =
                event->getIf<sf::Event::JoystickButtonPressed>()) {
            return processJoystickButtonEvent(*joystickPressed);
        } else if (const auto* joystickMoved =
                event->getIf<sf::Event::JoystickMoved>()) {
            return processJoystickAxisEvent(*joystickMoved);
        }
    }
    return event_t::NOTHING;
}

event_t SfmlEvent::processKeyboardEvent
    (const sf::Event::KeyPressed& keyPressed) {
    auto it = _reverseKeyMap.find(keyPressed.code);
    if (it != _reverseKeyMap.end()) {
        return it->second;
    }
    return event_t::NOTHING;
}

event_t SfmlEvent::processMouseEvent
    (const sf::Event::MouseButtonPressed& mousePressed) {
    for (const auto& pair : _mouseMap) {
        if (pair.second == mousePressed.button)
            return pair.first;
    }
    return event_t::MOUSECLICK;
}

event_t SfmlEvent::processJoystickButtonEvent
    (const sf::Event::JoystickButtonPressed& joystickPressed) {
    for (const auto& pair : _joystickButtonMap) {
        if (pair.second == joystickPressed.button)
            return pair.first;
    }
    return event_t::NOTHING;
}

event_t SfmlEvent::processJoystickAxisEvent
    (const sf::Event::JoystickMoved& joystickMoved) {
    if (joystickMoved.axis == sf::Joystick::Axis::PovX) {
        if (joystickMoved.position < -50.0f)
            return event_t::GAMEPAD_DPAD_LEFT;
        else if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_DPAD_RIGHT;
    } else if (joystickMoved.axis == sf::Joystick::Axis::PovY) {
        if (joystickMoved.position < -50.0f)
            return event_t::GAMEPAD_DPAD_DOWN;
        else if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_DPAD_UP;
    } else if (joystickMoved.axis == sf::Joystick::Axis::X) {
        if (joystickMoved.position < -50.0f)
            return event_t::GAMEPAD_LEFT_STICK_LEFT;
        else if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_LEFT_STICK_RIGHT;
    } else if (joystickMoved.axis == sf::Joystick::Axis::Y) {
        if (joystickMoved.position < -50.0f)
            return event_t::GAMEPAD_LEFT_STICK_DOWN;
        else if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_LEFT_STICK_UP;
    } else if (joystickMoved.axis == sf::Joystick::Axis::U) {
        if (joystickMoved.position < -50.0f)
            return event_t::GAMEPAD_RIGHT_STICK_LEFT;
        else if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_RIGHT_STICK_RIGHT;
    } else if (joystickMoved.axis == sf::Joystick::Axis::R) {
        if (joystickMoved.position < -50.0f)
            return event_t::GAMEPAD_RIGHT_STICK_DOWN;
        else if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_RIGHT_STICK_UP;
    } else if (joystickMoved.axis == sf::Joystick::Axis::Z) {
        if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_LEFT_TRIGGER;
    } else if (joystickMoved.axis == sf::Joystick::Axis::V) {
        if (joystickMoved.position > 50.0f)
            return event_t::GAMEPAD_RIGHT_TRIGGER;
    }
    return event_t::NOTHING;
}

void SfmlEvent::cleanup() {
    // Cleanup if needed
}

std::pair<int, int> SfmlEvent::getMousePos() {
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(_window);
    if (!sfmlWindow) return {0, 0};
    sf::Vector2i pos = sf::Mouse::getPosition(*sfmlWindow->getSfmlWindow());
    return {pos.x, pos.y};
}

bool SfmlEvent::isKeyPressed(event_t key) {
    auto it = _keyMap.find(key);
    if (it != _keyMap.end())
        return sf::Keyboard::isKeyPressed(it->second);
    auto jit = _joystickButtonMap.find(key);
    if (jit != _joystickButtonMap.end())
        return sf::Joystick::isButtonPressed(0, jit->second);

    return false;
}

bool SfmlEvent::isMouseButtonPressed(int button) {
    if (button == 0)
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    else if (button == 1)
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    return false;
}
