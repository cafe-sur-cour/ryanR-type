/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlEvent
*/

#ifndef SFMLEVENT_HPP_
#define SFMLEVENT_HPP_

#include <SFML/Graphics.hpp>
#include <map>
#include <chrono>
#include <string>
#include "../../../common/interfaces/IEvent.hpp"
#include "../../../common/interfaces/IWindow.hpp"
#include "SfmlKeyMappings.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"

class SfmlEvent : public gfx::IEvent {
    public:
        SfmlEvent(std::shared_ptr<ResourceManager> resourceManager, std::shared_ptr<gfx::IWindow> window);
        ~SfmlEvent() override;
        void init() override;
        event_t pollEvents() override;
        std::string getLastTextInput() override;
        void cleanup() override;
        std::pair<int, int> getMousePos() override;
        bool isKeyPressed(event_t key) override;
        bool isMouseButtonPressed(int button) override;
        float getAxisValue(event_t axis) override;

    private:
        void initializeMappings();
        void createReverseKeyboardMapping();
        event_t processKeyboardEvent(const sf::Event::KeyPressed &keyPressed);
        event_t processMouseEvent(const sf::Event::MouseButtonPressed &mousePressed);
        event_t processMouseReleaseEvent(const sf::Event::MouseButtonReleased &mouseReleased);
        event_t processJoystickButtonEvent(const sf::Event::JoystickButtonPressed &joystickPressed);
        event_t processJoystickAxisEvent(const sf::Event::JoystickMoved &joystickMoved);
        bool isJoystickAxisPressed(event_t key);

        // Member variables
        std::shared_ptr<ResourceManager> _resourceManager;
        std::shared_ptr<gfx::IWindow> _window;
        std::map<event_t, sf::Keyboard::Key> _keyMap;
        std::map<sf::Keyboard::Key, event_t> _reverseKeyMap;
        std::map<event_t, sf::Mouse::Button> _mouseMap;
        std::map<event_t, unsigned int> _joystickButtonMap;
        std::string _lastTextInput;
        std::chrono::steady_clock::time_point _lastTextTime;
        event_t _lastMouseWheelEvent = event_t::NOTHING;
};

#endif /* !SFMLEVENT_HPP_ */
