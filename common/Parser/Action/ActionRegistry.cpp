/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ActionRegistry
*/

#include <memory>
#include "ActionFactory.hpp"
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../constants.hpp"

void ActionFactory::initializeConditions() {
    registerAction(constants::DEATHINTENT_ACTION,
        [](std::shared_ptr<
            ecs::Registry> reg,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
    ) {
        (void)selfEntity;
        reg->addComponent<ecs::DeathIntentComponent>(otherEntity,
            std::make_shared<ecs::DeathIntentComponent>());
    });
}
