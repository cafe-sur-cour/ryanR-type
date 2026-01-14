/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationStateSyncSystem
*/

#include "AnimationStateSyncSystem.hpp"
#include <memory>
#include <string>
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/components/permanent/AnimationStateComponent.hpp"
#include "../../components/rendering/AnimationComponent.hpp"
#include "../../../common/debug.hpp"

namespace ecs {

void AnimationStateSyncSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;

    auto view = registry->view<AnimationStateComponent, AnimationComponent>();

    for (auto entity : view) {
        auto animStateComp = registry->getComponent<AnimationStateComponent>(entity);
        auto animComp = registry->getComponent<AnimationComponent>(entity);

        if (!animStateComp || !animComp) {
            continue;
        }

        std::string newState = animStateComp->getCurrentState();

        if (!newState.empty()) {
            animComp->setCurrentState(newState);
            animStateComp->setCurrentState("");
            registry->removeOneComponent<AnimationStateComponent>(entity);
        }
    }
}

}  // namespace ecs
