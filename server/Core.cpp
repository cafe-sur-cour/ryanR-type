/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/

#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

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

    while (this->_server->getState() != 0) {
        processServerEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Core::processServerEvents() {
    if (this->_server == nullptr) {
        return;
    }

    if (!this->_server->hasEvents()) {
        return;
    }

    auto eventQueue = this->_server->getEventQueue();
    if (!eventQueue) {
        return;
    }

    while (!eventQueue->empty()) {
        auto event = eventQueue->front();
        eventQueue->pop();
        uint8_t clientId = event.first;
        constants::EventType eventType = event.second;
        std::cout << "[CORE] Processing event from client " << static_cast<int>(clientId)
                  << ": " << static_cast<int>(eventType) << std::endl;
    }
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
