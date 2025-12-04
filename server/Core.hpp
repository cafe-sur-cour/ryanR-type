/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/

#include "Server.hpp"
#include "Utils.hpp"
#include "ServerConfig.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include <thread>

#ifndef CORE_HPP_
#define CORE_HPP_

class Core {
    public:
        Core();
        ~Core();

        void init();
        void loop();
        void processServerEvents();

        std::shared_ptr<rserv::Server> getServer() const;
        std::shared_ptr<rserv::ServerConfig> getConfig() const;
        std::shared_ptr<ResourceManager> getResourceManager() const;

    protected:
    private:
        std::shared_ptr<Utils> _utils;
        std::shared_ptr<rserv::Server> _server;
        std::thread _serverThread;

        std::shared_ptr<ResourceManager> _resourceManager;
};

#endif /* !CORE_HPP_ */
