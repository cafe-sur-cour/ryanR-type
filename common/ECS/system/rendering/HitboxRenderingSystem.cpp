/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HitboxRenderingSystem
*/

#include <memory>
#include "HitboxRenderingSystem.hpp"
#include "../../component/rendering/HitboxRenderComponent.hpp"
#include "../../component/permanent/TransformComponent.hpp"
#include "../../component/permanent/ColliderComponent.hpp"
#include "../../view/View.hpp"
#include "../../resourceManager/ResourceManager.hpp"
#include "../../../../libs/Multimedia/IWindow.hpp"


namespace ecs {

HitboxRenderingSystem::HitboxRenderingSystem() {
}

void HitboxRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) {
    (void)deltaTime;

    View<HitboxRenderComponent, TransformComponent,
        ColliderComponent> view(registry);

    for (auto entityId : view) {
        auto hitbox = registry->getComponent<HitboxRenderComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);
        auto colliders = registry->getComponents<ColliderComponent>(entityId);

        if (!hitbox || !transform || colliders.empty())
            continue;

        auto collider = colliders[0];
        math::FRect hitboxRect = collider->getHitbox(transform->getPosition());

        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const gfx::color_t& color = hitbox->getColor();

            window->drawRectangleOutline(color,
                {static_cast<size_t>(hitboxRect.getLeft()),
                    static_cast<size_t>(hitboxRect.getTop())},
                {static_cast<size_t>(hitboxRect.getWidth()),
                    static_cast<size_t>(hitboxRect.getHeight())});
        }
    }
}

}  // namespace ecs
