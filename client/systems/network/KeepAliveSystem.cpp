/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** KeepAliveSystem
*/

#include "KeepAliveSystem.hpp"
#include <memory>
#include "../../ClientNetwork.hpp"
#include "../../../common/constants.hpp"

namespace ecs {

KeepAliveSystem::KeepAliveSystem() : _pingInterval(5.0f) {
    _lastPingTime = std::chrono::steady_clock::now();
}

void KeepAliveSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)registry;
    (void)deltaTime;

    if (!resourceManager->has<ClientNetwork>()) {
        return;
    }

    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastPing = std::chrono::duration_cast<
        std::chrono::duration<float>>(now - _lastPingTime).count();

    if (timeSinceLastPing >= _pingInterval) {
        auto clientNetwork = resourceManager->get<ClientNetwork>();

        NetworkEvent keepAliveEvent;
        keepAliveEvent.eventType = constants::EventType::NO_OP;
        keepAliveEvent.depth = 0.0;

        clientNetwork->addToEventQueue(keepAliveEvent);
        _lastPingTime = now;
    }
}

}
