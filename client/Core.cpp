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
#include "initRessourcesManager/initRessourcesManager.hpp"
#include "gsm/states/scenes/DevState.hpp"

Core::Core() {
    _resourceManager = initRessourcesManager();

    _gsm = std::make_shared<gsm::GameStateMachine>();
    std::shared_ptr<gsm::DevState> devState =
        std::make_shared<gsm::DevState>(_gsm, _resourceManager);
    _gsm->changeState(devState);

    initNetwork();
    _networkThread = std::thread(&Core::networkLoop, this);
}

Core::~Core() {
    if (_networkThread.joinable()) {
        _networkThread.join();
    }
}

void Core::run() {
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (_resourceManager->get<gfx::IWindow>()->isOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        _gsm->update(deltaTime.count());
        _gsm->render();
    }
}

void Core::initNetwork() {
    // TODO(anyone): Initialize client network
    std::cout << "Network initialized (placeholder)" << std::endl;
}

void Core::networkLoop() {
    // TODO(anyone): Implement network loop
    std::cout << "Network thread running (placeholder)" << std::endl;
}
