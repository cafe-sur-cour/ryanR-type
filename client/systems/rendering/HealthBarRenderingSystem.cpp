/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HealthBarRenderingSystem
*/

#include <memory>
#include "HealthBarRenderingSystem.hpp"
#include "../../constants.hpp"
#include "../../../common/components/permanent/HealthComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"

namespace ecs {

HealthBarRenderingSystem::HealthBarRenderingSystem() {
}

void HealthBarRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    View<HealthComponent, TransformComponent, ColliderComponent> view(registry);

    for (auto entityId : view) {
        auto health = registry->getComponent<HealthComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);
        auto collider = registry->getComponent<ColliderComponent>(entityId);

        if (!health || !transform || !collider)
            continue;

        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const math::Vector2f& pos = transform->getPosition();

            math::FRect hitbox = collider->getHitbox(pos, transform->getScale());

            float barX = hitbox.getLeft();
            float barY = hitbox.getTop() + constants::HEALTH_BAR_OFFSET_Y;
            float barWidth = hitbox.getWidth();
            float barHeight = constants::HEALTH_BAR_HEIGHT;

            float outlineThickness = constants::HEALTH_BAR_OUTLINE_THICKNESS;
            window->drawFilledRectangle({0, 0, 0, 255},
                {static_cast<size_t>(barX - outlineThickness),
                    static_cast<size_t>(barY - outlineThickness)},
                {static_cast<size_t>(barWidth + 2 * outlineThickness),
                    static_cast<size_t>(barHeight + 2 * outlineThickness)});

            float healthRatio = health->getHealth() / health->getBaseHealth();
            if (healthRatio < 0.0f) healthRatio = 0.0f;
            if (healthRatio > 1.0f) healthRatio = 1.0f;

            uint8_t red = static_cast<uint8_t>((1.0f - healthRatio) * 255);
            uint8_t green = static_cast<uint8_t>(healthRatio * 255);
            uint8_t blue = 0;

            float fillWidth = barWidth * healthRatio;
            window->drawFilledRectangle({red, green, blue, 255},
                {static_cast<size_t>(barX), static_cast<size_t>(barY)},
                {static_cast<size_t>(fillWidth), static_cast<size_t>(barHeight)});
        }
    }
}

}  // namespace ecs
