/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Core
*/


#include <memory>
#include <chrono>

#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "ClientNetwork.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "gsm/states/scenes/DevState.hpp"

#ifndef CORE_HPP_
#define CORE_HPP_

class Core {
    public:
        Core(std::shared_ptr<ClientNetwork> clientNetwork);
        ~Core();

        void gameLoop();
    protected:
    private:
        std::chrono::duration<float> _deltaTime;

        std::shared_ptr<ClientNetwork> _clientNetwork;
        std::shared_ptr<ecs::ResourceManager> _resourceManager;
        std::shared_ptr<gsm::GameStateMachine> _gsm;
        std::shared_ptr<gsm::DevState> _devState;
};

#endif /* !CORE_HPP_ */
