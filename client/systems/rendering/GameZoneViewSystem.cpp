/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneViewSystem
*/

#include <memory>
#include "GameZoneViewSystem.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "../../constants.hpp"

namespace ecs {

GameZoneViewSystem::GameZoneViewSystem() {
}

void GameZoneViewSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {

    View<GameZoneComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto gameZoneComp = registry->getComponent<GameZoneComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!gameZoneComp || !transform)
            continue;

        math::FRect zone = gameZoneComp->getZone();
        math::Vector2f position = transform->getPosition();

        float centerX = position.getX() + zone.getLeft() + zone.getWidth() / 2.0f;
        float centerY = position.getY() + zone.getTop() + zone.getHeight() / 2.0f;
        math::Vector2f targetCenter(centerX, centerY);

        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            math::Vector2f currentCenter = window->getViewCenter();

            float smoothingSpeed = constants::VIEW_SMOOTHING_SPEED;
            math::Vector2f direction = targetCenter - currentCenter;
            math::Vector2f newCenter = currentCenter + direction * smoothingSpeed * deltaTime;

            window->setViewCenter(newCenter.getX(), newCenter.getY());
        }
        break;
    }
}

}  // namespace ecs
