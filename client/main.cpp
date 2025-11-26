/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>
#include <chrono>
#include "initRessourcesManager/initRessourcesManager.hpp"
#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "../client/graphicals/IWindow.hpp"
#include "../client/graphicals/IEvent.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "gsm/states/scenes/DevState.hpp"
#include "graphicals/EventTypes.hpp"
#include "ClientNetwork.hpp"
#include "Utils.hpp"


int main(int ac, char **av) {
    Utils utils;
    std::shared_ptr<ClientNetwork> clientNetwork =
        std::make_shared<ClientNetwork>();
    std::shared_ptr<ecs::ResourceManager> resourceManager =
        initRessourcesManager();

    // tmp loop to test window and events
    utils.parseCli(ac, av, clientNetwork);
    std::shared_ptr<gsm::GameStateMachine> gsm =
        std::make_shared<gsm::GameStateMachine>();
    std::shared_ptr<gsm::DevState> devState =
        std::make_shared<gsm::DevState>(gsm, resourceManager);
    gsm->changeState(devState);
    auto previousTime = std::chrono::high_resolution_clock::now();
    while (resourceManager->get<gfx::IWindow>()->isOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;
        previousTime = currentTime;

        gsm->update(deltaTime.count());
        gsm->render();
    }
    return 0;
}
