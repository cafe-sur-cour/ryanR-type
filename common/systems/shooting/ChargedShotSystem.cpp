/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotSystem
*/

#include "ChargedShotSystem.hpp"
#include "../../components/permanent/ChargedShotComponent.hpp"
#include <iostream>
namespace ecs {

ChargedShotSystem::ChargedShotSystem() {
}

void ChargedShotSystem::update(
    [[maybe_unused]] std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    auto entities = registry->view<ChargedShotComponent>();

    for (auto entity : entities) {
        auto comp = registry->getComponent<ChargedShotComponent>(entity);

        comp->setCharge(comp->getCharge() + deltaTime);
    }
}

}
