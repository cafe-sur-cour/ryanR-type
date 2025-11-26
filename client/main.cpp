/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>
#include "initRessourcesManager/initRessourcesManager.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "../client/graphicals/IWindow.hpp"
#include "../client/graphicals/IEvent.hpp"
#include "graphicals/EventTypes.hpp"
#include "ClientNetwork.hpp"
#include "Utils.hpp"

int main(int ac, char **av) {
    Utils utils;
    std::shared_ptr<ClientNetwork> clientNetwork = std::make_shared<ClientNetwork>();
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        initRessourcesManager();

    // tmp loop to test window and events
    utils.parseCli(ac, av, clientNetwork);
    while (resourceManager->get<gfx::IWindow>()->isOpen()) {
        // tmp to see events
        auto eventResult = resourceManager->get<gfx::IEvent>()->pollEvents();
        if (eventResult == gfx::EventType::CLOSE) {
            std::cout << "Event: Window closed" << std::endl;
            break;
        } else if (eventResult != gfx::EventType::NOTHING) {
            std::cout << "press" << static_cast<int>(eventResult) << std::endl;
        }
        resourceManager->get<gfx::IWindow>()->clear();
        resourceManager->get<gfx::IWindow>()->drawRectangle(
            gfx::color_t{255, 0, 0}, {100, 100}, {200, 150});
        resourceManager->get<gfx::IWindow>()->display();
    }
    return 0;
}
