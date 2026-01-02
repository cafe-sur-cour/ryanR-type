/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ActionFactory
*/

#include "ActionFactory.hpp"
#include <memory>
#include <string>
#include <iostream>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../components/temporary/DamageIntentComponent.hpp"
#include "../../components/permanent/DamageComponent.hpp"
#include "../../constants.hpp"
#include "../../components/permanent/HealthComponent.hpp"

const ActionFactory& ActionFactory::getInstance() {
    static ActionFactory instance;
    return instance;
}

ActionFactory::ActionFactory() {
    initializeConditions();
}

void ActionFactory::registerAction(const std::string& actionId, ActionFunction action) {
    _actions[actionId] = action;
}

void ActionFactory::executeAction(
    const std::string& actionId,
    std::shared_ptr<ecs::Registry> registry,
    ecs::Entity self,
    ecs::Entity other
) const {
    auto it = _actions.find(actionId);
    if (it != _actions.end()) {
        it->second(registry, self, other);
    }
}

bool ActionFactory::hasAction(const std::string& actionId) const {
    return _actions.find(actionId) != _actions.end();
}

void ActionFactory::initializeConditions() {
    registerAction(constants::DEALDEATH_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)selfEntity;
            reg->addComponent<ecs::DeathIntentComponent>(otherEntity,
                std::make_shared<ecs::DeathIntentComponent>(selfEntity));
        });

    registerAction(constants::TAKEDEATH_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)otherEntity;
            reg->addComponent<ecs::DeathIntentComponent>(selfEntity,
                std::make_shared<ecs::DeathIntentComponent>(otherEntity));
        });

    registerAction(constants::DEALDAMAGE_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            auto damageComp = reg->getComponent<ecs::DamageComponent>(selfEntity);
            if (damageComp) {
                float damage = damageComp->getDamage();
                reg->addComponent<ecs::DamageIntentComponent>(otherEntity,
                    std::make_shared<ecs::DamageIntentComponent>(damage, selfEntity));
            }
        });

    registerAction(constants::TAKEDAMAGE_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)otherEntity;
            auto damageComp = reg->getComponent<ecs::DamageComponent>(selfEntity);
            if (damageComp) {
                float damage = damageComp->getDamage();
                reg->addComponent<ecs::DamageIntentComponent>(selfEntity,
                    std::make_shared<ecs::DamageIntentComponent>(damage, selfEntity));
            }
        });

    registerAction(constants::ADDLIFE_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            auto lifeCompSelf = reg->getComponent<ecs::HealthComponent>(selfEntity);
            auto lifeCompOther = reg->getComponent<ecs::HealthComponent>(otherEntity);

            if (lifeCompSelf && lifeCompOther) {
                lifeCompOther->increaseHealth(lifeCompSelf->getHealth());
                lifeCompSelf->setHealth(0);
            }
        });
}
