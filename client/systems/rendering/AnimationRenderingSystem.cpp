/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationRenderingSystem
*/

#include <memory>
#include <iostream>
#include <algorithm>
#include "AnimationRenderingSystem.hpp"
#include "../../components/rendering/AnimationComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/ECS/entity/Entity.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"

namespace ecs {

AnimationRenderingSystem::AnimationRenderingSystem() {
}

void AnimationRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {

    View<AnimationComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto animation = registry->getComponent<AnimationComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!animation || !transform || !animation->isValid())
            continue;

        for (const auto& transition : animation->getTransitions()) {
            if (transition.from == animation->getCurrentState() &&
                transition.condition(registry, entityId)
            ) {
                animation->setCurrentState(transition.to);
                break;
            }
        }

        if (!animation->isPlaying())
            continue;

        animation->setTimer(animation->getTimer() + deltaTime);

        std::shared_ptr<const AnimationClip> clip = animation->getCurrentClip();
        if (!clip)
            continue;

        int frameIndex;
        if (clip->loop) {
            frameIndex = static_cast<int>(animation->getTimer() / clip->speed)
                % clip->frameCount;
        } else {
            frameIndex = std::min(static_cast<int>(animation->getTimer() /
                clip->speed), clip->frameCount - 1);
        }
        animation->setCurrentFrame(frameIndex);

        float frameX = clip->startWidth + static_cast<float>(frameIndex) * clip->frameWidth;
        float frameY = clip->startHeight;
        math::FRect frameRect(frameX, frameY, clip->frameWidth, clip->frameHeight);
        animation->setFrameRect(frameRect);

        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const math::Vector2f& pos = transform->getPosition();
            const math::Vector2f& scale = transform->getScale();
            window->drawSprite(clip->texturePath,
                pos.getX(), pos.getY(), frameRect, scale.getX(), scale.getY());
        }
    }
}

}  // namespace ecs
