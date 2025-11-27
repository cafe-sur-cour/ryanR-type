/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationRenderingSystem
*/

#include <memory>
#include "AnimationRenderingSystem.hpp"
#include "../../component/permanent/AnimationComponent.hpp"
#include "../../component/permanent/TransformComponent.hpp"
#include "../../view/View.hpp"
#include "../../../../client/graphicals/IWindow.hpp"
#include <iostream>

namespace ecs {

AnimationRenderingSystem::AnimationRenderingSystem() {
}

void AnimationRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) {

    (void)deltaTime;

    View<AnimationComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto animation = registry->getComponent<AnimationComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!animation || !transform || !animation->isValid())
            continue;
        float elapsedTime = animation->getChrono().getElapsedSeconds();
        int frameIndex = static_cast<int>(elapsedTime /
            animation->getAnimationSpeed()) % animation->getFrameCount();
        animation->setCurrentFrame(frameIndex);

        float frameX = animation->getStartWidth() + static_cast<float>(frameIndex) *
            animation->getFrameWidth();
        float frameY = animation->getStartHeight();
        math::FRect frameRect(frameX, frameY, animation->getFrameWidth(),
                            animation->getFrameHeight());
        animation->setFrameRect(frameRect);

        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const math::Vector2f& pos = transform->getPosition();
            const math::Vector2f& scale = transform->getScale();
            window->drawSprite(animation->getTexturePath(),
                pos.getX(), pos.getY(), frameRect, scale.getX(), scale.getY());
        }
    }
}

}  // namespace ecs
