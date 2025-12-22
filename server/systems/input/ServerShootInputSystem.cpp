/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerShootInputSystem
*/

#include <memory>
#include <vector>
#include <iostream>
#include "ServerShootInputSystem.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/components/tags/ShooterTag.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../../common/systems/SystemNames.hpp"

namespace ecs {

ServerShootInputSystem::ServerShootInputSystem() {
}

void ServerShootInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)deltaTime;

    if (!resourceManager->has<ecs::ServerInputProvider>()) {
        return;
    }

    auto serverInputProvider = resourceManager->get<ecs::ServerInputProvider>();

    auto view = registry->view<ControllableTag, ShooterTag, PlayerTag>();

    std::vector<size_t> connectedClients = serverInputProvider->getConnectedClients();
    if (connectedClients.empty())
        return;

    size_t clientIndex = 0;
    for (auto entityId : view) {
        size_t clientID = connectedClients[clientIndex];
        float value = serverInputProvider->getActionAxis(InputAction::SHOOT, clientID);
        if (value > 0.0f) {
            updateShootIntent(registry, entityId);
            serverInputProvider->setAxisValue(InputAction::SHOOT, 0.0f, clientID);
        }
        clientIndex++;
    }
}

void ServerShootInputSystem::updateShootIntent(
    std::shared_ptr<Registry> registry,
    Entity entityId) {
    registry->registerComponent<ShootIntentComponent>();

    if (!registry->hasComponent<ShootIntentComponent>(entityId)) {
        auto shootIntentComponent = std::make_shared<ShootIntentComponent>();
        registry->addComponent(entityId, shootIntentComponent);
    }
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::ServerShootInputSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::SERVER_SHOOT_INPUT_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
