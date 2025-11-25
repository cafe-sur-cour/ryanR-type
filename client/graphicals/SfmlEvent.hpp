/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlEvent
*/

#ifndef SFMLEVENT_HPP_
#define SFMLEVENT_HPP_

#include <SFML/Graphics.hpp>
#include "./IEvent.hpp"
#include "./IWindow.hpp"
#include "../../common/ECS/resourceManager/ResourceManager.hpp"

class SfmlEvent : public gfx::IEvent {
    public:
        SfmlEvent(std::shared_ptr<ecs::ResourceManager> resourceManager, std::shared_ptr<gfx::IWindow> window);
        ~SfmlEvent() override;
        void init() override;
        event_t pollEvents(std::pair<int, int> gridSize) override;
        void cleanup() override;
        std::pair<int, int> getMousePos() override;
        bool isKeyPressed(event_t key) override;
        bool isMouseButtonPressed(int button) override;
        std::string getTypedText() override;
    private:
        std::shared_ptr<ecs::ResourceManager> _resourceManager;
        std::shared_ptr<gfx::IWindow> _window;
        std::string _typedText;
};

#endif /* !SFMLEVENT_HPP_ */