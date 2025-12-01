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
#include "initRessourcesManager/initRessourcesManager.hpp"
#include "gsm/states/scenes/DevState.hpp"
#include "../../common/Signal/Signal.hpp"
#include "../../libs/Multimedia/IWindow.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../common/DLLoader/DLLoader.hpp"

Core::Core() {
    Signal::setupSignalHandlers();

    initLibraries();

    this->_resourceManager = initRessourcesManager(
        this->_windowLoader,
        this->_eventLoader
    );

    this->_gsm = std::make_shared<gsm::GameStateMachine>();
    std::shared_ptr<gsm::DevState> devState =
        std::make_shared<gsm::DevState>(this->_gsm, this->_resourceManager);
    this->_gsm->changeState(devState);

    initNetwork();
    this->_networkThread = std::thread(&Core::networkLoop, this);
}

Core::~Core() {
    if (this->_networkThread.joinable()) {
        this->_networkThread.join();
        this->_clientNetwork->stop();
    }
    this->_windowLoader->Close();
    this->_eventLoader->Close();
}

void Core::run() {
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (this->_resourceManager->get<gfx::IWindow>()->isOpen()
        && !Signal::stopFlag) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        this->_gsm->update(deltaTime.count());
        this->_gsm->render();
    }

    Signal::stopFlag = 1;
}

void Core::initNetwork() {
    this->_clientNetwork = std::make_shared<ClientNetwork>();
}

void Core::initLibraries() {
    std::string multimediaPath = std::string(pathLoad) + "/" + multimediaLib;

    _windowLoader = std::make_shared<DLLoader<gfx::createWindow_t>>(
        DLLoader<gfx::createWindow_t>()
    );
    _eventLoader = std::make_shared<DLLoader<gfx::createEvent_t>>(
        DLLoader<gfx::createEvent_t>()
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
}

void Core::networkLoop() {
    this->_clientNetwork->start();
}


std::shared_ptr<ClientNetwork> Core::getNetwork() {
    return this->_clientNetwork;
}
