/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/

#include <memory>

#include "Core.hpp"
#include "initResourcesManager/initResourcesManager.hpp"

Core::Core() {
    this->_utils = std::make_shared<Utils>();
    this->_server = std::make_shared<rserv::Server>();

    this->_resourceManager = std::make_shared<ResourceManager>();
}

Core::~Core() {
    if (this->_serverThread.joinable()) {
        if (this->_server != nullptr && this->_server->getState() == 1) {
            this->_server->stop();
        }
        this->_serverThread.join();
    }
    if (this->_resourceManager != nullptr) {
        this->_resourceManager.reset();
    }
    if (this->_server != nullptr) {
        this->_server.reset();
    }
    if (this->_utils != nullptr) {
        this->_utils.reset();
    }
}

void Core::init() {
    this->_resourceManager = initResourcesManager();
}

void Core::loop() {
    this->_serverThread = std::thread([this]() {
        this->_server->init();
        this->_server->start();
    });
}

std::shared_ptr<rserv::ServerConfig> Core::getConfig() const {
    return this->_server->getConfig();
}

std::shared_ptr<rserv::Server> Core::getServer() const {
    return this->_server;
}

std::shared_ptr<ResourceManager> Core::getResourceManager() const {
    return this->_resourceManager;
}
