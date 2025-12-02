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
#include <queue>
#include <mutex>
#include <condition_variable>
#include "../common/ECS/resourceManager/ResourceManager.hpp"
#include "ClientNetwork.hpp"
#include "../libs/Multimedia/IWindow.hpp"
#include "../libs/Multimedia/IEvent.hpp"
#include "gsm/machine/GameStateMachine.hpp"
#include "../common/DLLoader/DLLoader.hpp"

struct NetworkEvent {
    constants::EventType eventType;
    double depth;
    double direction;
};

class Core
{
    public:
        Core();
        ~Core();

        void run();
        void startNetwork();

        std::shared_ptr<ClientNetwork> getNetwork();

    private:
        std::shared_ptr<DLLoader<gfx::createWindow_t>> _windowLoader;
        std::shared_ptr<DLLoader<gfx::createEvent_t>> _eventLoader;

        std::shared_ptr<ecs::ResourceManager> _resourceManager;
        std::shared_ptr<gsm::GameStateMachine> _gsm;
        std::shared_ptr<ClientNetwork> _clientNetwork;
        std::thread _networkThread;

        void initNetwork();
        void initLibraries();
        void networkLoop();

        std::queue<NetworkEvent> _eventQueue;
        std::mutex _queueMutex;
        std::condition_variable _queueCond;
};

#endif /* !CORE_HPP_ */
