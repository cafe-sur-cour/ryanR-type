/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationRenderingSystem
*/

#include <memory>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "AnimationRenderingSystem.hpp"
#include "../../components/rendering/AnimationComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/ECS/entity/Entity.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "../../../common/Parser/Animation/AnimationConditionFactory.hpp"
#include "../../../common/constants.hpp"
#include "../../../common/systems/SystemNames.hpp"

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

        if (!animation->getStateJustChanged()) {
            for (const auto& transition : animation->getTransitions()) {
                if (transition.from == animation->getCurrentState()) {
                    bool allConditionsMet = true;
                    for (const auto& condition : transition.conditions) {
                        bool value = AnimationConditionFactory::getConditionValue(
                            condition.param,
                            registry,
                            entityId);
                        if (value != condition.equals) {
                            allConditionsMet = false;
                            break;
                        }
                    }
                    if (allConditionsMet) {
                        if (transition.playRewind) {
                            std::shared_ptr<const AnimationClip> currentClip =
                                animation->getCurrentClip();
                            if (!currentClip)
                                continue;

                            if (!animation->isPlayingRewind()) {
                                animation->setRewindStartFrame(animation->getCurrentFrame());
                                animation->setPlayingRewind(true);
                                animation->setTimer(0.0f);
                            } else {
                                if (animation->getCurrentFrame() == 0) {
                                    static std::unordered_map<Entity, float> waitTimers;
                                    if (waitTimers.find(entityId) == waitTimers.end())
                                        waitTimers[entityId] = 0.0f;

                                    waitTimers[entityId] += deltaTime;

                                    if (waitTimers[entityId] >= currentClip->speed) {
                                        animation->setCurrentState(transition.to);
                                        animation->setPlayingRewind(false);
                                        waitTimers.erase(entityId);
                                    }
                                }
                            }
                        } else {
                            animation->setCurrentState(transition.to);
                        }
                        break;
                    }
                }
            }
        } else {
            if (animation->getTimer() >= animation->getMinAnimationTime()) {
                animation->setStateJustChanged(false);
            }
        }

        if (!animation->isPlaying())
            continue;

        std::shared_ptr<const AnimationClip> clip = animation->getCurrentClip();
        if (!clip)
            continue;

        animation->setTimer(animation->getTimer() + deltaTime);

        int frameIndex;
        if (animation->isPlayingRewind()) {
            int framesPassed = static_cast<int>(animation->getTimer() / clip->speed);
            int startFrame = animation->getRewindStartFrame();

            frameIndex = startFrame - framesPassed;

            if (frameIndex < 0)
                frameIndex = 0;
        } else {
            if (clip->loop) {
                frameIndex = static_cast<int>(animation->getTimer() /
                    clip->speed) % clip->frameCount;
            } else {
                frameIndex = std::min(static_cast<int>(animation->getTimer() /
                    clip->speed), clip->frameCount - 1);
            }
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

extern "C" ecs::ISystem* createSystem() {
    return new ecs::AnimationRenderingSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::ANIMATION_RENDERING_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
