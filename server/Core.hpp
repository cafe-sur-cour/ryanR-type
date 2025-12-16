/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/

#ifndef CORE_HPP_
#define CORE_HPP_

#include <memory>
#include <stack>
#include "Server.hpp"
#include "Utils.hpp"
#include "ServerConfig.hpp"
#include "../common/resourceManager/ResourceManager.hpp"
#include <thread>
#include "../common/ECS/entity/registry/Registry.hpp"
#include "../common/Parser/Parser.hpp"
#include "../common/systems/systemManager/ISystemManager.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "initResourcesManager/ServerInputProvider.hpp"

class Core {
    public:
        Core();
        ~Core();

        void init();
        void loop();

        std::shared_ptr<rserv::Server> getServer() const;
        std::shared_ptr<rserv::ServerConfig> getConfig() const;
        std::shared_ptr<ResourceManager> getResourceManager() const;
        std::shared_ptr<ecs::Registry> getRegistry() const;
        std::shared_ptr<Parser> getParser() const;
        std::shared_ptr<ecs::ISystemManager> getSystemsManager() const;
        std::shared_ptr<gsm::GameStateMachine> getGameStateMachine() const;
        void processServerEvents();

    protected:
    private:
        std::shared_ptr<Utils> _utils;
        std::shared_ptr<rserv::Server> _server;
        std::thread _serverThread;

        std::shared_ptr<ResourceManager> _resourceManager;
        std::shared_ptr<ecs::Registry> _registry;
        std::shared_ptr<Parser> _parser;
        std::shared_ptr<ecs::ISystemManager> _systemsManager;
        std::shared_ptr<gsm::GameStateMachine> _gsm;
        std::shared_ptr<ecs::ServerInputProvider> _inputProvider;
        float _statusUpdateTimer;
};

#endif /* !CORE_HPP_ */
