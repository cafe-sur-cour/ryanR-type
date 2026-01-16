/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core.cpp
*/

#include "Core.hpp"
#include <iostream>
#include <chrono>
#include <memory>
#include <string>
#include "initResourcesManager/initResourcesManager.hpp"
#include "gsm/states/scenes/SplashScreen/SplashScreenState.hpp"
#include "../common/debug.hpp"
#include "../../common/Signal/Signal.hpp"
#include "../../common/interfaces/IWindow.hpp"
#include "../../common/interfaces/IEvent.hpp"
#include "../../common/DLLoader/DLLoader.hpp"
#include "../../common/Error/LibrairiesLoadError.hpp"

Core::Core() {
    Signal::setupSignalHandlers();

    initLibraries();
    initNetwork();
    this->_registry = std::make_shared<ecs::Registry>();
    this->_gsm = std::make_shared<gsm::GameStateMachine>();
    auto entityPrefabManager = std::make_shared<EntityPrefabManager>();
    this->_parser = std::make_shared<Parser>(
        entityPrefabManager,
        ParsingType::CLIENT,
        this->_registry
    );
    _parser->parseAllEntities(constants::CONFIG_PATH);
    this->_resourceManager = initResourcesManager(
        this->_windowLoader,
        this->_eventLoader,
        this->_audioLoader,
        this->_clientNetwork,
        this->_parser
    );
    this->_clientNetwork->setResourceManager(this->_resourceManager);
    this->_clientNetwork->setGameStateMachine(this->_gsm);
    this->_resourceManager->add<ecs::Registry>(this->_registry);
    this->_resourceManager->add<EntityPrefabManager>(entityPrefabManager);
    this->_lastHealthcheckTime = std::chrono::steady_clock::now();
}

Core::~Core() {
    if (this->_clientNetwork != nullptr) {
        this->_clientNetwork->stop();
    }

    if (this->_networkThread.joinable()) {
        this->_networkThread.join();
    }

    if (this->_clientNetwork != nullptr) {
        this->_clientNetwork.reset();
    }

    if (this->_registry != nullptr) {
        this->_registry.reset();
    }

    if (this->_gsm != nullptr) {
        this->_gsm.reset();
    }

    if (this->_parser != nullptr) {
        this->_parser.reset();
    }

    if (this->_resourceManager != nullptr) {
        this->_resourceManager->clear();
        this->_resourceManager.reset();
    }

    if (this->_windowLoader != nullptr) {
        this->_windowLoader->Close();
        this->_windowLoader.reset();
    }

    if (this->_eventLoader != nullptr) {
        this->_eventLoader->Close();
        this->_eventLoader.reset();
    }

    if (this->_audioLoader != nullptr) {
        this->_audioLoader->Close();
        this->_audioLoader.reset();
    }
}

void Core::initFirstScene() {
    std::shared_ptr<gsm::SplashScreenState> splashScreenState =
        std::make_shared<gsm::SplashScreenState>(_gsm, _resourceManager);
    _gsm->changeState(splashScreenState);
}

void Core::run() {
    debug::Debug::printDebug(this->_clientNetwork->isDebugMode(),
        "Entering main loop",
        debug::debugType::CORE,
        debug::debugLevel::INFO);
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (this->_resourceManager->get<gfx::IWindow>()->isOpen()
        && !Signal::stopFlag) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        if (this->_clientNetwork && this->_clientNetwork->isConnected()) {
            auto now = std::chrono::steady_clock::now();
            auto timeSinceLastHealthcheck = std::chrono::duration_cast<
                std::chrono::duration<float>>(now - _lastHealthcheckTime).count();
            if (timeSinceLastHealthcheck >= _healthcheckInterval) {
                NetworkEvent healthcheckEvent;
                healthcheckEvent.eventType = constants::EventType::HEALTHCHECK;
                healthcheckEvent.depth = 0.0;
                this->_clientNetwork->addToEventQueue(healthcheckEvent);
                _lastHealthcheckTime = now;
            }
        }

        this->_resourceManager->get<gfx::IWindow>()->clear();
        this->_gsm->update(deltaTime.count());
        this->_resourceManager->get<gfx::IWindow>()->display();
    }

    Signal::stopFlag = 1;
}

void Core::initNetwork() {
    this->_clientNetwork = std::make_shared<ClientNetwork>();
}

void Core::startNetwork() {
    this->_networkThread = std::thread(&Core::networkLoop, this);
}

void Core::initLibraries() {
    std::string multimediaPath = std::string(pathLoad) + "/" + multimediaLib sharedLibExt;

    _windowLoader = std::make_shared<DLLoader<gfx::createWindow_t>>(
        DLLoader<gfx::createWindow_t>()
    );
    _eventLoader = std::make_shared<DLLoader<gfx::createEvent_t>>(
        DLLoader<gfx::createEvent_t>()
    );
    _audioLoader = std::make_shared<DLLoader<gfx::createAudio_t>>(
        DLLoader<gfx::createAudio_t>()
    );

    if (!this->_windowLoader->Open(multimediaPath.c_str())) {
        std::string error = this->_windowLoader->Error();
        std::string errorMsg = "Failed to load libMultimedia for window: ";
        errorMsg += multimediaPath;
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw err::LibrairiesLoadError(errorMsg,
            err::LibrairiesLoadError::LIBRARY_NOT_FOUND);
    }
    if (!this->_eventLoader->Open(multimediaPath.c_str())) {
        std::string error = this->_eventLoader->Error();
        std::string errorMsg = "Failed to load libMultimedia for events: ";
        errorMsg += multimediaPath;
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw err::LibrairiesLoadError(errorMsg,
            err::LibrairiesLoadError::LIBRARY_NOT_FOUND);
    }
    if (!this->_audioLoader->Open(multimediaPath.c_str())) {
        std::string error = this->_audioLoader->Error();
        std::string errorMsg = "Failed to load libMultimedia for audio: ";
        errorMsg += multimediaPath;
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw err::LibrairiesLoadError(errorMsg,
            err::LibrairiesLoadError::LIBRARY_NOT_FOUND);
    }
}

void Core::networkLoop() {
    this->_clientNetwork->init();
    this->_clientNetwork->start();
}

std::shared_ptr<ClientNetwork> Core::getNetwork() {
    return this->_clientNetwork;
}
