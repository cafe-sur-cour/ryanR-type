/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ShootInputSystem
*/

#include "ShootInputSystem.hpp"
#include <memory>
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/components/tags/ShooterTag.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/systems/SystemNames.hpp"
#include "../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../ClientNetwork.hpp"
#include "../../constants.hpp"

namespace ecs {

ShootInputSystem::ShootInputSystem() {
}

void ShootInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    if (!resourceManager->has<IInputProvider>())
        return;

    auto inputProvider = resourceManager->get<IInputProvider>();
    if (!inputProvider->isActionPressed(InputAction::SHOOT))
        return;

    auto playerView = registry->view<ControllableTag, ShooterTag, LocalPlayerTag>();

    for (auto playerId : playerView) {
        if (!registry->hasComponent<ShootIntentComponent>(playerId)) {
            auto shootIntentComponent = std::make_shared<ShootIntentComponent>();
            registry->addComponent<ShootIntentComponent>(playerId, shootIntentComponent);
        }
        auto clientNetwork = resourceManager->get<ClientNetwork>();
        if (clientNetwork) {
            NetworkEvent shootEvent;
            shootEvent.eventType = constants::EventType::SHOOT;
            shootEvent.depth = static_cast<double>(1);
            clientNetwork->addToEventQueue(shootEvent);
        }
    }
}

}

extern "C" ecs::ISystem* createSystem() {
    return new ecs::ShootInputSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::SHOOT_INPUT_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
