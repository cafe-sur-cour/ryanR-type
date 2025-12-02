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
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../../common/components/temporary/ShootIntentComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"

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
    if (inputProvider->isActionPressed(InputAction::SHOOT))
        return;

    auto playerView = registry->view<ControllableTag, ShooterTag, PlayerTag>();

    for (auto playerId : playerView) {
        auto playerTransform = registry->getComponent<TransformComponent>(playerId);

        auto shootIntentComponent = std::make_shared<ShootIntentComponent>(
            playerTransform->getPosition()
        );
        if (!registry->hasComponent<ShootIntentComponent>(playerId))
            registry->addComponent<ShootIntentComponent>(playerId, shootIntentComponent);
    }
}

}
