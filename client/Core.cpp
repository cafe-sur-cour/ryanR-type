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
#include "gsm/states/scenes/MainMenu/MainMenuState.hpp"
#include "../common/debug.hpp"
#include "../../common/Signal/Signal.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../common/DLLoader/DLLoader.hpp"

Core::Core() {
    Signal::setupSignalHandlers();

    initLibraries();
    initNetwork();
    this->_resourceManager = initResourcesManager(
        this->_windowLoader,
        this->_eventLoader,
        this->_audioLoader,
        this->_clientNetwork
    );

    this->_gsm = std::make_shared<gsm::GameStateMachine>();
    std::shared_ptr<gsm::MainMenuState> mainMenuState =
        std::make_shared<gsm::MainMenuState>(this->_gsm, this->_resourceManager);
    this->_gsm->changeState(mainMenuState);
}

Core::~Core() {
    if (this->_networkThread.joinable()) {
        this->_networkThread.join();
        this->_clientNetwork->stop();
    }
    this->_windowLoader->Close();
    this->_eventLoader->Close();
    this->_audioLoader->Close();
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
        throw std::runtime_error(errorMsg);
    }
    if (!this->_eventLoader->Open(multimediaPath.c_str())) {
        std::string error = this->_eventLoader->Error();
        std::string errorMsg = "Failed to load libMultimedia for events: ";
        errorMsg += multimediaPath;
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw std::runtime_error(errorMsg);
    }
    if (!this->_audioLoader->Open(multimediaPath.c_str())) {
        std::string error = this->_audioLoader->Error();
        std::string errorMsg = "Failed to load libMultimedia for audio: ";
        errorMsg += multimediaPath;
        if (!error.empty()) {
            errorMsg += " - Error: " + error;
        }
        throw std::runtime_error(errorMsg);
    }
}

void Core::networkLoop() {
    this->_clientNetwork->init();

    if (this->_clientNetwork->getConnectionState()
        == net::ConnectionState::CONNECTING) {
        debug::Debug::printDebug(this->_clientNetwork->isDebugMode(),
        "Sending Connection request",
        debug::debugType::NETWORK,
        debug::debugLevel::INFO);
        this->_clientNetwork->connectionPacket();
    }
    this->_clientNetwork->start();
}

std::shared_ptr<ClientNetwork> Core::getNetwork() {
    return this->_clientNetwork;
}
