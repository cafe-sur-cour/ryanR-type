/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlEvent
*/

#include <utility>
#include <string>
#include <memory>
#include <iostream>
#include "SfmlWindow.hpp"
#include "SfmlEvent.hpp"
#include "../../../common/constants.hpp"

using event_t = gfx::IEvent::event_t;

SfmlEvent::SfmlEvent(std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<gfx::IWindow> window)
    : _resourceManager(resourceManager), _window(window), _lastTextInput("") {
    this->_keyMap = {};
    this->_reverseKeyMap = {};
    this->_mouseMap = {};
    this->_joystickButtonMap = {};
    init();
}

SfmlEvent::~SfmlEvent() {}

void SfmlEvent::init() {
    initializeMappings();
    createReverseKeyboardMapping();
    _lastTextTime = std::chrono::steady_clock::now();
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
    bool isFocused = window->hasFocus();
    event_t lastEvent = event_t::NOTHING;

    while (auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            return event_t::CLOSE;
        } else if (event->is<sf::Event::Resized>()) {
            sfmlWindow->updateView();
        } else if (auto textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (!isFocused) continue;
            auto now = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
                now - _lastTextTime
            );
            if (duration.count() > 100) {
                _lastTextInput = std::string(1, static_cast<char>(textEntered->unicode));
                _lastTextTime = now;
                lastEvent = event_t::TEXT_INPUT;
            }
        } else if (auto keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (!isFocused)
                continue;
            lastEvent = processKeyboardEvent(*keyPressed);
        } else if (auto mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (!isFocused) continue;
            lastEvent = processMouseEvent(*mousePressed);
        } else if (auto mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (!isFocused) continue;
            lastEvent = processMouseReleaseEvent(*mouseReleased);
        } else if (auto joystickPressed = event->getIf<sf::Event::JoystickButtonPressed>()) {
            if (!isFocused) continue;
            lastEvent = processJoystickButtonEvent(*joystickPressed);
        }
    }
    return lastEvent;
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

event_t SfmlEvent::processMouseReleaseEvent
    (const sf::Event::MouseButtonReleased& mouseReleased) {
    for (const auto& pair : _mouseMap) {
        if (pair.second == mouseReleased.button) {
            if (pair.first == event_t::MOUSELEFTCLICK)
                return event_t::MOUSELEFTRELEASE;
            else if (pair.first == event_t::MOUSERIGHTCLICK)
                return event_t::MOUSERIGHTRELEASE;
            else if (pair.first == event_t::MOUSEMIDDLECLICK)
                return event_t::MOUSEMIDDLERELEASE;
        }
    }
    return event_t::NOTHING;
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
    const float deadzoneThreshold = constants::GAMEPAD_DEADZONE * 100.0f;

    if (joystickMoved.axis == sf::Joystick::Axis::PovX) {
        if (joystickMoved.position < -deadzoneThreshold)
            return event_t::GAMEPAD_DPAD_LEFT;
        else if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_DPAD_RIGHT;
    } else if (joystickMoved.axis == sf::Joystick::Axis::PovY) {
        if (joystickMoved.position < -deadzoneThreshold)
            return event_t::GAMEPAD_DPAD_UP;
        else if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_DPAD_DOWN;
    } else if (joystickMoved.axis == sf::Joystick::Axis::X) {
        if (joystickMoved.position < -deadzoneThreshold)
            return event_t::GAMEPAD_LEFT_STICK_LEFT;
        else if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_LEFT_STICK_RIGHT;
    } else if (joystickMoved.axis == sf::Joystick::Axis::Y) {
        if (joystickMoved.position < -deadzoneThreshold)
            return event_t::GAMEPAD_LEFT_STICK_DOWN;
        else if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_LEFT_STICK_UP;
    } else if (joystickMoved.axis == sf::Joystick::Axis::U) {
        if (joystickMoved.position < -deadzoneThreshold)
            return event_t::GAMEPAD_RIGHT_STICK_LEFT;
        else if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_RIGHT_STICK_RIGHT;
    } else if (joystickMoved.axis == sf::Joystick::Axis::R) {
        if (joystickMoved.position < -deadzoneThreshold)
            return event_t::GAMEPAD_RIGHT_STICK_DOWN;
        else if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_RIGHT_STICK_UP;
    } else if (joystickMoved.axis == sf::Joystick::Axis::Z) {
        if (joystickMoved.position > deadzoneThreshold)
            return event_t::GAMEPAD_LEFT_TRIGGER;
    } else if (joystickMoved.axis == sf::Joystick::Axis::V) {
        if (joystickMoved.position > deadzoneThreshold)
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
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(_window);
    if (!sfmlWindow || !sfmlWindow->getSfmlWindow()->hasFocus())
        return false;
    auto it = _keyMap.find(key);
    if (it != _keyMap.end())
        return sf::Keyboard::isKeyPressed(it->second);
    auto jit = _joystickButtonMap.find(key);
    if (jit != _joystickButtonMap.end())
        return sf::Joystick::isButtonPressed(0, jit->second);

    return isJoystickAxisPressed(key);
}

bool SfmlEvent::isJoystickAxisPressed(event_t key) {
    const float deadzoneThreshold = constants::GAMEPAD_DEADZONE * 100.0f;
    float axisValue = 0.0f;
    bool result = false;

    switch (key) {
        case event_t::GAMEPAD_LEFT_STICK_LEFT:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
            result = axisValue < -deadzoneThreshold;
            break;
        case event_t::GAMEPAD_LEFT_STICK_RIGHT:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_LEFT_STICK_UP:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
            result = axisValue < -deadzoneThreshold;
            break;
        case event_t::GAMEPAD_LEFT_STICK_DOWN:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_RIGHT_STICK_LEFT:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
            result = axisValue < -deadzoneThreshold;
            break;
        case event_t::GAMEPAD_RIGHT_STICK_RIGHT:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_RIGHT_STICK_UP:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
            result = axisValue < -deadzoneThreshold;
            break;
        case event_t::GAMEPAD_RIGHT_STICK_DOWN:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_LEFT_TRIGGER:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_RIGHT_TRIGGER:
            axisValue = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_DPAD_LEFT:
            axisValue =
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
            result = axisValue < -deadzoneThreshold;
            break;
        case event_t::GAMEPAD_DPAD_RIGHT:
            axisValue =
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
            result = axisValue > deadzoneThreshold;
            break;
        case event_t::GAMEPAD_DPAD_UP:
            axisValue =
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
            result = axisValue < -deadzoneThreshold;
            break;
        case event_t::GAMEPAD_DPAD_DOWN:
            axisValue =
                sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
            result = axisValue > deadzoneThreshold;
            break;
        default:
            return false;
    }

    return result;
}

bool SfmlEvent::isMouseButtonPressed(int button) {
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(_window);
    if (!sfmlWindow || !sfmlWindow->getSfmlWindow()->hasFocus())
        return false;
    if (button == 0)
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    else if (button == 1)
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    return false;
}

float SfmlEvent::getAxisValue(event_t axis) {
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(_window);
    if (!sfmlWindow || !sfmlWindow->getSfmlWindow()->hasFocus())
        return 0.0f;
    switch (axis) {
        case event_t::GAMEPAD_LEFT_STICK_LEFT:
        case event_t::GAMEPAD_LEFT_STICK_RIGHT:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
        case event_t::GAMEPAD_LEFT_STICK_UP:
        case event_t::GAMEPAD_LEFT_STICK_DOWN:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);
        case event_t::GAMEPAD_RIGHT_STICK_LEFT:
        case event_t::GAMEPAD_RIGHT_STICK_RIGHT:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::U);
        case event_t::GAMEPAD_RIGHT_STICK_UP:
        case event_t::GAMEPAD_RIGHT_STICK_DOWN:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::R);
        case event_t::GAMEPAD_LEFT_TRIGGER:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Z);
        case event_t::GAMEPAD_RIGHT_TRIGGER:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::V);
        case event_t::GAMEPAD_DPAD_LEFT:
        case event_t::GAMEPAD_DPAD_RIGHT:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
        case event_t::GAMEPAD_DPAD_UP:
        case event_t::GAMEPAD_DPAD_DOWN:
            return sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);
        default:
            return 0.0f;
    }
}

std::string SfmlEvent::getLastTextInput() {
    return _lastTextInput;
}
