/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/

#include <chrono>

#include "initRessourcesManager/initRessourcesManager.hpp"
#include "../client/graphicals/IWindow.hpp"
#include "../client/graphicals/IEvent.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "graphicals/EventTypes.hpp"
#include "Core.hpp"

Core::Core(std::shared_ptr<ClientNetwork> clientNetwork) {
    this->_clientNetwork = clientNetwork;
    this->_resourceManager = std::make_shared<ecs::ResourceManager>();
    this->_resourceManager = initRessourcesManager();
    this->_gsm = std::make_shared<gsm::GameStateMachine>();
    this->_devState = std::make_shared<gsm::DevState>(this->_gsm,
        this->_resourceManager);
}

Core::~Core() {
}

void Core::gameLoop() {

    this->_gsm->changeState(this->_devState);
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (this->_resourceManager->get<gfx::IWindow>()->isOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        this->_deltaTime = currentTime - previousTime;
        previousTime = currentTime;
        this->_gsm->update(this->_deltaTime.count());
        this->_gsm->render();
    }
}
