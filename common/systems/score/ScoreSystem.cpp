/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreSystem
*/

#include "ScoreSystem.hpp"
#include <iostream>
#include <memory>
#include "../../components/permanent/ScoreComponent.hpp"
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../ECS/view/View.hpp"
#include "../../components/temporary/ScoreIntentComponent.hpp"
#include "../SystemNames.hpp"

namespace ecs {

ScoreSystem::ScoreSystem() {
}

ScoreSystem::~ScoreSystem() {
}

void ScoreSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;
    auto view = registry->view<ScoreComponent>();

    for (auto entityId : view) {
        auto scoreComp = registry->getComponent<ScoreComponent>(entityId);
        if (scoreComp) {
            auto intentComp = registry->getComponent<ScoreIntentComponent>(entityId);
            if (intentComp) {
                scoreComp->addScore(intentComp->getScore());
                registry->removeOneComponent<ScoreIntentComponent>(entityId);
            }
        }
    }
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::ScoreSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::SCORE_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
