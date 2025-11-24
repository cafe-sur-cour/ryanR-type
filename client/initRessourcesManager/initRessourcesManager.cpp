/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initRessourcesManager
*/

#include "../../common/ECS/resourceManager/ResourceManager.hpp"
#include <iostream>

std::shared_ptr<ecs::ResourceManager> initRessourcesManager()
{
    std::shared_ptr<ecs::ResourceManager> resourceManager = std::make_shared<ecs::ResourceManager>();

    // Initialize and load resources here
    // window
    // networking
    // etc.
    return resourceManager;
}
