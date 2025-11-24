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

std::shared_ptr<ecs::ResourceManager> initRessourcesManager() {
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        std::make_shared<ecs::ResourceManager>();

    // Initialize and load resources here
    // window
    // networking
    // etc.
    return resourceManager;
}
