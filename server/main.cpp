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
#include "../common/Error/IError.hpp"
#include "ServerConfig.hpp"


int main(int ac, char **av) {
    Utils utils;
    std::shared_ptr<rserv::ServerConfig> config =
        std::make_shared<rserv::ServerConfig>();
    std::shared_ptr<rserv::IServer> server = std::make_shared<rserv::Server>();

    utils.parsCli(ac, av, config);
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        initRessourcesManager();
    try {
        server->setConfig(config);
        server->init();
        server->start();
    } catch (const err::IError &e) {
        std::cerr << e.getDetails() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "Unknown error occurred in server." << std::endl;
        return 84;
    }
}
