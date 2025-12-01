/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpriteRenderingSystem
*/

#include <memory>
#include "SpriteRenderingSystem.hpp"
#include "../../components/rendering/SpriteComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/ECS/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"


namespace ecs {

SpriteRenderingSystem::SpriteRenderingSystem() {
}

void SpriteRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    View<SpriteComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto sprite = registry->getComponent<SpriteComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!sprite || !transform || !sprite->isValid())
            continue;
        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const math::Vector2f& pos = transform->getPosition();
            const math::Vector2f& scale = transform->getScale();
            window->drawSprite(sprite->getTexturePath(),
                pos.getX(), pos.getY(), scale.getX(), scale.getY());
        }
    }
}

}  // namespace ecs
