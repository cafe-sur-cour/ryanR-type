/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneViewSystem
*/

#include <memory>
#include <cmath>
#include <algorithm>
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
            float distance = std::sqrt(direction.getX() * direction.getX() +
                direction.getY() * direction.getY());

            if (distance > 0.1f) {
                float maxDistancePerSecond = smoothingSpeed * 100.0f;
                float moveDistance = std::min(distance, maxDistancePerSecond * deltaTime);
                math::Vector2f normalizedDirection = direction * (1.0f / distance);
                math::Vector2f newCenter = currentCenter + normalizedDirection * moveDistance;
                window->setViewCenter(newCenter.getX(), newCenter.getY());
            } else {
                window->setViewCenter(targetCenter.getX(), targetCenter.getY());
            }
        }
        break;
    }
}

}  // namespace ecs
