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
#include <chrono>
#include "../common/resourceManager/ResourceManager.hpp"
#include "ClientNetwork.hpp"
#include "../common/interfaces/IWindow.hpp"
#include "../common/interfaces/IEvent.hpp"
#include "../common/interfaces/IAudio.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "../common/DLLoader/DLLoader.hpp"
#include "../common/Parser/Parser.hpp"

class Core
{
    public:
        Core();
        ~Core();

        void initFirstScene();
        void run();
        void startNetwork();

        std::shared_ptr<ClientNetwork> getNetwork();

    private:
        std::shared_ptr<DLLoader<gfx::createWindow_t>> _windowLoader;
        std::shared_ptr<DLLoader<gfx::createEvent_t>> _eventLoader;
        std::shared_ptr<DLLoader<gfx::createAudio_t>> _audioLoader;

        std::shared_ptr<ResourceManager> _resourceManager;
        std::shared_ptr<gsm::GameStateMachine> _gsm;
        std::shared_ptr<ecs::Registry> _registry;
        std::shared_ptr<ClientNetwork> _clientNetwork;
        std::shared_ptr<Parser> _parser;
        std::thread _networkThread;

        std::chrono::steady_clock::time_point _lastHealthcheckTime;
        float _healthcheckInterval = constants::HEALTHCHECK_INTERVAL;

        void initNetwork();
        void initLibraries();
        void networkLoop();
};

#endif /* !CORE_HPP_ */
