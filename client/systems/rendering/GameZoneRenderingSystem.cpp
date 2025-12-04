/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GameZoneRenderingSystem
*/

#include <memory>
#include "GameZoneRenderingSystem.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
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

            gfx::color_t color = {255, 0, 0};

            window->drawFilledRectangle(color,
                {static_cast<size_t>(zone.getLeft()),
                    static_cast<size_t>(zone.getTop())},
                {static_cast<size_t>(zone.getWidth()), 1});
            window->drawFilledRectangle(color,
                {static_cast<size_t>(zone.getLeft()),
                    static_cast<size_t>(zone.getTop() + zone.getHeight() - 1)},
                {static_cast<size_t>(zone.getWidth()), 1});
            window->drawFilledRectangle(color,
                {static_cast<size_t>(zone.getLeft()),
                    static_cast<size_t>(zone.getTop())},
                {1, static_cast<size_t>(zone.getHeight())});
            window->drawFilledRectangle(color,
                {static_cast<size_t>(zone.getLeft() + zone.getWidth() - 1),
                    static_cast<size_t>(zone.getTop())},
                {1, static_cast<size_t>(zone.getHeight())});
        }
    }
}

}  // namespace ecs
