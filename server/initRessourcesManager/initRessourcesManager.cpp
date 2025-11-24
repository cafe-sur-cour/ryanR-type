/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initRessourcesManager
*/

#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../ServerConfig.hpp"
#include <memory>
#include <iostream>

std::shared_ptr<ecs::ResourceManager> initRessourcesManager()
{
    std::shared_ptr<ecs::ResourceManager> resourceManager = std::make_shared<ecs::ResourceManager>();

    // Initialize and load resources here
    // exemple resourceManager->add(std::make_shared<rserv::ServerConfig>(8080));
    // networking
    // etc.
    return resourceManager;
}
