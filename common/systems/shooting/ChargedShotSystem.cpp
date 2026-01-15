/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotSystem
*/

#include "ChargedShotSystem.hpp"
#include <memory>
#include "../../components/permanent/ChargedShotComponent.hpp"

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

        float reloadTime = comp->getReloadTime();
        float maxCharge = comp->getMaxCharge();

        float chargeIncrement = (reloadTime > 0.0f)
            ? (deltaTime * maxCharge / reloadTime)
            : deltaTime;

        comp->setCharge(
            (std::min)(
                comp->getCharge() + chargeIncrement,
                maxCharge
            )
        );
    }
}

}
