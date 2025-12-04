/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneRenderingSystem
*/

#include <memory>
#include "GameZoneRenderingSystem.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/ColliderComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "../../../common/constants.hpp"

namespace ecs {

GameZoneRenderingSystem::GameZoneRenderingSystem() {
}

void GameZoneRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    View<GameZoneComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto gameZoneComp = registry->getComponent<GameZoneComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!gameZoneComp || !transform)
            continue;

        math::FRect zone = gameZoneComp->getZone();
        zone.setLeft(zone.getLeft() + transform->getPosition().getX());

        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();

            gfx::color_t zoneColor = {255, 0, 0, 50};
            window->drawFilledRectangle(zoneColor,
                {static_cast<size_t>(zone.getLeft()),
                    static_cast<size_t>(zone.getTop())},
                {static_cast<size_t>(zone.getWidth()),
                    static_cast<size_t>(zone.getHeight())});

            gfx::color_t boundaryColor = {0, 255, 0, 100};

            window->drawFilledRectangle(boundaryColor,
                {static_cast<size_t>(zone.getLeft()),
                    static_cast<size_t>(-constants::GAME_ZONE_BOUNDARY_THICKNESS)},
                {static_cast<size_t>(zone.getWidth()),
                    static_cast<size_t>(constants::GAME_ZONE_BOUNDARY_THICKNESS)});

            window->drawFilledRectangle(boundaryColor,
                {static_cast<size_t>(zone.getLeft()),
                    static_cast<size_t>(constants::MAX_HEIGHT)},
                {static_cast<size_t>(zone.getWidth()),
                    static_cast<size_t>(constants::GAME_ZONE_BOUNDARY_THICKNESS)});

            auto colliders = registry->getComponents<ColliderComponent>(entityId);
            math::Vector2f entityScale = transform->getScale();
            math::Vector2f entityPos = transform->getPosition();

            for (auto& collider : colliders) {
                math::FRect hitbox = collider->getHitbox(entityPos, entityScale);

                gfx::color_t colliderColor;
                if (collider->getType() == CollisionType::Push) {
                    colliderColor = {0, 0, 255, 150};
                } else if (collider->getType() == CollisionType::Solid) {
                    colliderColor = {0, 255, 0, 150};
                } else {
                    continue;
                }

                window->drawFilledRectangle(colliderColor,
                    {static_cast<size_t>(hitbox.getLeft()),
                        static_cast<size_t>(hitbox.getTop())},
                    {static_cast<size_t>(hitbox.getWidth()),
                        static_cast<size_t>(hitbox.getHeight())});
            }
        }
    }
}

}  // namespace ecs
