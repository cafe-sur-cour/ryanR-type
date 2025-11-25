/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initRessourcesManager
*/

#include <iostream>
#include <memory>
#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include "initRessourcesManager.hpp"
#include "../graphicals/IWindow.hpp"
#include "../graphicals/IEvent.hpp"
#include "../graphicals/SfmlWindow.hpp"
#include "../graphicals/SfmlEvent.hpp"

std::shared_ptr<ecs::ResourceManager> initRessourcesManager() {
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        std::make_shared<ecs::ResourceManager>();

    std::shared_ptr<SfmlWindow> window = std::make_shared<SfmlWindow>();
    resourceManager->add<gfx::IWindow>(window);
    resourceManager->add<gfx::IEvent>(std::make_shared<SfmlEvent>(resourceManager, window));
    return resourceManager;
}
