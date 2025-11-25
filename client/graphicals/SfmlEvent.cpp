/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlEvent
*/

#include "SfmlEvent.hpp"
#include "SfmlWindow.hpp"

SfmlEvent::SfmlEvent(std::shared_ptr<ecs::ResourceManager> resourceManager, std::shared_ptr<gfx::IWindow> window)
    : _resourceManager(resourceManager), _window(window), _typedText("") {
    init();
}

SfmlEvent::~SfmlEvent() {}

void SfmlEvent::init() {
    _typedText.clear();
}

gfx::IEvent::event_t SfmlEvent::pollEvents(std::pair<int, int> gridSize) {
    (void)gridSize;
    auto sfmlWindow = std::dynamic_pointer_cast<SfmlWindow>(_window);
    if (!sfmlWindow)
        return NOTHING;
    auto window = sfmlWindow->getSfmlWindow();
    while (auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            return CLOSE;
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            switch (keyPressed->code) {
                case sf::Keyboard::Key::Up:
                    return UP;
                case sf::Keyboard::Key::Down:
                    return DOWN;
                case sf::Keyboard::Key::Left:
                    return LEFT;
                case sf::Keyboard::Key::Right:
                    return RIGHT;
                case sf::Keyboard::Key::Space:
                    return SPACE;
                case sf::Keyboard::Key::Enter:
                    return ENTER;
                case sf::Keyboard::Key::Escape:
                    return ESCAPE;
                default:
                    break;
            }
        }
        else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mousePressed->button == sf::Mouse::Button::Left)
                return MOUSELEFTCLICK;
            else if (mousePressed->button == sf::Mouse::Button::Right)
                return MOUSERIGHTCLICK;
            return MOUSECLICK;
        }
        else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
            if (textEntered->unicode < 128) {
                _typedText += static_cast<char>(textEntered->unicode);
                return TYIPING;
            }
        }
    }
    return NOTHING;
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
    switch (key) {
        case UP:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
        case DOWN:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);
        case LEFT:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
        case RIGHT:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
        case SPACE:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
        case ENTER:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
        case ESCAPE:
            return sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape);
        default:
            return false;
    }
}

bool SfmlEvent::isMouseButtonPressed(int button) {
    if (button == 0)
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    else if (button == 1)
        return sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    return false;
}

std::string SfmlEvent::getTypedText() {
    std::string text = _typedText;
    _typedText.clear();
    return text;
}