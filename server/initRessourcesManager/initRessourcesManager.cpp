/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** initRessourcesManager
*/

#include <memory>
#include <iostream>
#include "../../common/resourceManager/ResourceManager.hpp"
#include "ServerInputProvider.hpp"
#include "../ServerConfig.hpp"
#include "initRessourcesManager.hpp"

std::shared_ptr<ResourceManager> initRessourcesManager() {
    std::shared_ptr<ResourceManager> resourceManager =
        std::make_shared<ResourceManager>();

    // Initialize and load resources here
    // exemple resourceManager->
    // add(std::make_shared<rserv::ServerConfig>(8080));
    // networking
    // etc.

    auto inputProvider = std::make_shared<ecs::ServerInputProvider>();
    resourceManager->add<ecs::IInputProvider>(inputProvider);

    return resourceManager;
}
