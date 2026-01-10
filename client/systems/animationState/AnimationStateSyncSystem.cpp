/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationStateSyncSystem
*/

#include "AnimationStateSyncSystem.hpp"
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/components/permanent/AnimationStateComponent.hpp"
#include "../../components/rendering/AnimationComponent.hpp"
#include "../../../common/debug.hpp"
#include <iostream>
namespace ecs {

void AnimationStateSyncSystem::update(std::shared_ptr<ResourceManager> resourceManager,
                                      std::shared_ptr<Registry> registry,
                                      float deltaTime) {
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
            auto it = _lastAppliedState.find(entity);
            if (it == _lastAppliedState.end() || it->second != newState) {
                debug::Debug::printDebug(true,
                    "[AnimationStateSync] Entity " + std::to_string(entity) +
                    " changing animation state to: " + newState,
                    debug::debugType::ECS,
                    debug::debugLevel::INFO);

                animComp->setCurrentState(newState);
                _lastAppliedState[entity] = newState;
            }
        }
    }
}

}  // namespace ecs
