/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core.hpp
*/

#ifndef CORE_HPP_
#define CORE_HPP_

#include <memory>
#include <thread>
#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "graphicals/IWindow.hpp"
#include "gsm/machine/GameStateMachine.hpp"

class Core {
public:
    Core();
    ~Core();

    void run();

private:
    std::shared_ptr<ecs::ResourceManager> _resourceManager;
    std::shared_ptr<gsm::GameStateMachine> _gsm;
    std::thread _networkThread;

    void initNetwork();
    void networkLoop();
};

#endif /* !CORE_HPP_ */
