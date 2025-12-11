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
#include "../common/debug.hpp"
#include "initResourcesManager/initResourcesManager.hpp"
#include "gsm/states/scenes/Boot/BootState.hpp"
#include "../../common/systems/systemManager/SystemManager.hpp"
#include "../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"

Core::Core() {
    this->_utils = std::make_shared<Utils>();
    this->_server = std::make_shared<rserv::Server>();

    this->_registry = std::make_shared<ecs::Registry>();
    this->_systemsManager = std::make_shared<ecs::SystemManager>();
    this->_gsm = std::make_shared<gsm::GameStateMachine>();

    auto entityPrefabManager = std::make_shared<EntityPrefabManager>();
    this->_parser = std::make_shared<Parser>(
        entityPrefabManager,
        ParsingType::SERVER,
        this->_registry
    );

    this->_inputProvider = std::make_shared<ecs::ServerInputProvider>();

    this->_resourceManager = initResourcesManager(
        this->_server,
        this->_registry,
        this->_parser,
        this->_systemsManager,
        this->_gsm,
        this->_inputProvider
    );
    this->_parser->parseAllEntities(constants::CONFIG_PATH);
    this->_parser->parseMapFromFile("configs/map/map1.json");
    this->_server->setCurrentMap(this->_parser->getMapParser()->createPacketFromMap());
}

Core::~Core() {
    if (this->_serverThread.joinable()) {
        if (this->_server != nullptr && this->_server->getState() == SERVER_UP) {
            this->_server->stop();
        }
        this->_serverThread.join();
    }
    if (this->_gsm != nullptr) {
        this->_gsm.reset();
    }
    if (this->_systemsManager != nullptr) {
        this->_systemsManager.reset();
    }
    if (this->_parser != nullptr) {
        this->_parser.reset();
    }
    if (this->_registry != nullptr) {
        this->_registry.reset();
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
    std::shared_ptr<gsm::BootState> bootState = std::make_shared<gsm::BootState>(
        this->_gsm,
        this->_resourceManager
    );
    this->_gsm->changeState(bootState);
    this->_serverThread = std::thread([this]() {
        this->_server->init();
        this->_server->start();
    });
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
        uint8_t clientId = std::get<0>(event);
        constants::EventType eventType = std::get<1>(event);
        double param1 = std::get<2>(event);
        this->_inputProvider->updateInputFromEvent
            (clientId, eventType, static_cast<float>(param1));
    }
}

void Core::loop() {
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (this->_server->getState() < SERVER_UP) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SERVER_THREAD_SLEEP_MS));
    }

    // Wait for all clients to connect and game to start
    while (this->_server->getState() == SERVER_UP && !this->_server->isGameStarted()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(SERVER_THREAD_SLEEP_MS));
    }

    while (this->_server->getState() == SERVER_UP) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - previousTime).count();
        previousTime = currentTime;
        processServerEvents();
        this->_gsm->update(deltaTime);
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

std::shared_ptr<ecs::Registry> Core::getRegistry() const {
    return this->_registry;
}

std::shared_ptr<Parser> Core::getParser() const {
    return this->_parser;
}

std::shared_ptr<ecs::ISystemManager> Core::getSystemsManager() const {
    return this->_systemsManager;
}

std::shared_ptr<gsm::GameStateMachine> Core::getGameStateMachine() const {
    return this->_gsm;
}
