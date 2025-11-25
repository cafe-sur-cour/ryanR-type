/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main
*/

#include <iostream>
#include <memory>

#include "initRessourcesManager/initRessourcesManager.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "ServerConfig.hpp"


int main(int ac, char **av) {
    Utils utils;
    std::shared_ptr<rserv::ServerConfig> config = std::make_shared<rserv::ServerConfig>();

    utils.parsCli(ac, av, config);
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        initRessourcesManager();

}
