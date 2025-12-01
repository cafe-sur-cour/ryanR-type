/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** CoinDropSystem
*/

#include "CoinDropSystem.hpp"
#include "../../../common/ECS/resourceManager/IInputProvider.hpp"
#include "../../../common/ECS/resourceManager/InputAction.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/components/tags/CoinTag.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/temporary/SoundIntentComponent.hpp"
#include "../../components/rendering/RectangleRenderComponent.hpp"

namespace ecs {

CoinDropSystem::CoinDropSystem() : _wasPressed(false) {
}

void CoinDropSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)deltaTime;

    if (!resourceManager->has<IInputProvider>()) {
        return;
    }

    auto inputProvider = resourceManager->get<IInputProvider>();
    bool isPressed = inputProvider->isActionPressed(InputAction::DROP_COIN);

    if (isPressed && !_wasPressed) {
        auto playerView = registry->view<PlayerTag, TransformComponent>();

        for (auto playerId : playerView) {
            auto playerTransform = registry->getComponent<TransformComponent>(playerId);
            if (!playerTransform) {
                continue;
            }

            Entity coinId = registry->createEntity();
            
            auto coinTransform = std::make_shared<TransformComponent>(
                playerTransform->getPosition(),
                0.0f,
                math::Vector2f(1.0f, 1.0f)
            );
            registry->addComponent(coinId, coinTransform);
            
            auto coinTag = std::make_shared<CoinTag>();
            registry->addComponent(coinId, coinTag);

            auto coinRender = std::make_shared<RectangleRenderComponent>(
                gfx::color_t{255, 215, 0},
                20.0f,
                20.0f
            );
            registry->addComponent(coinId, coinRender);

            auto soundIntent = std::make_shared<SoundIntentComponent>(
                "sounds/coin-sound.wav",
                30.0f
            );
            registry->addComponent(coinId, soundIntent);
        }
    }

    _wasPressed = isPressed;
}

}  // namespace ecs
