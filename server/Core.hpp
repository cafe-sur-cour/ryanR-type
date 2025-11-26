/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/

#include "Server.hpp"
#include "Utils.hpp"
#include "ServerConfig.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"

#ifndef CORE_HPP_
#define CORE_HPP_

class Core {
    public:
        Core();
        ~Core();

        void init();
        void loop();

        std::shared_ptr<rserv::ServerConfig> getConfig() const;
        std::shared_ptr<rserv::Server> getServer() const;
        std::shared_ptr<ecs::ResourceManager> getResourceManager() const;

    protected:
    private:
        std::shared_ptr<Utils> _utils;
        std::shared_ptr<rserv::ServerConfig> _config;
        std::shared_ptr<rserv::Server> _server;

        /* Add esc related elem here */
        std::shared_ptr<ecs::ResourceManager> _resourceManager;
};

#endif /* !CORE_HPP_ */
