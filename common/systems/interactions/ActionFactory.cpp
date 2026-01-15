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
#include "../../components/permanent/ScriptingComponent.hpp"
#include "../../components/permanent/DamageCooldownComponent.hpp"
#include "../../GameRules.hpp"
#include "../../resourceManager/ResourceManager.hpp"

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
    std::shared_ptr<ResourceManager> resourceManager,
    ecs::Entity self,
    ecs::Entity other
) const {
    auto it = _actions.find(actionId);
    if (it != _actions.end()) {
        it->second(registry, resourceManager, self, other);
    }
}

bool ActionFactory::hasAction(const std::string& actionId) const {
    return _actions.find(actionId) != _actions.end();
}

void ActionFactory::initializeConditions() {
    registerAction(constants::DEALDEATH_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)resourceManager;
            (void)selfEntity;
            reg->addComponent<ecs::DeathIntentComponent>(otherEntity,
                std::make_shared<ecs::DeathIntentComponent>(selfEntity));
        });

    registerAction(constants::TAKEDEATH_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)resourceManager;
            (void)otherEntity;
            reg->addComponent<ecs::DeathIntentComponent>(selfEntity,
                std::make_shared<ecs::DeathIntentComponent>(otherEntity));
        });

    registerAction(constants::DEALDAMAGE_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            auto damageComp = reg->getComponent<ecs::DamageComponent>(selfEntity);
            if (damageComp) {
                float damage = damageComp->getDamage();
                auto gameRules = resourceManager->get<GameRules>();
                if (gameRules) {
                    GameRulesNS::Difficulty diff = gameRules->getDifficulty();
                    float multiplier = constants::DIFFICULTY_NORMAL_MULTIPLIER;
                    if (diff == GameRulesNS::Difficulty::EASY) multiplier =
                        constants::DIFFICULTY_EASY_MULTIPLIER;
                    else if (diff == GameRulesNS::Difficulty::HARD) multiplier =
                        constants::DIFFICULTY_HARD_MULTIPLIER;
                    bool isPlayerRelated = TagRegistry::getInstance().hasTag(
                            reg, selfEntity, constants::PLAYERTAG) ||
                        TagRegistry::getInstance().hasTag(
                            reg, selfEntity, constants::PLAYERPROJECTILETAG);
                    bool isEnemyRelated = TagRegistry::getInstance().hasTag(
                            reg, selfEntity, constants::MOBTAG) ||
                        TagRegistry::getInstance().hasTag(
                            reg, selfEntity, constants::ENNEMYPROJECTILETAG);
                    if (isPlayerRelated) {
                        damage *= multiplier;
                    } else if (isEnemyRelated) {
                        damage *= (2.0f - multiplier);
                    }
                }
                reg->addComponent<ecs::DamageIntentComponent>(otherEntity,
                    std::make_shared<ecs::DamageIntentComponent>(damage, selfEntity));
                auto cooldownComp =
                    reg->getComponent<ecs::DamageCooldownComponent>(otherEntity);
                if (cooldownComp) {
                    cooldownComp->setLastDamageTime(0.0f);
                }
            }
        });

    registerAction(constants::TAKEDAMAGE_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            auto damageComp = reg->getComponent<ecs::DamageComponent>(selfEntity);
            if (damageComp) {
                float damage = damageComp->getDamage();
                auto gameRules = resourceManager->get<GameRules>();
                if (gameRules) {
                    GameRulesNS::Difficulty diff = gameRules->getDifficulty();
                    float multiplier = constants::DIFFICULTY_NORMAL_MULTIPLIER;
                    if (diff == GameRulesNS::Difficulty::EASY) multiplier =
                        constants::DIFFICULTY_EASY_MULTIPLIER;
                    else if (diff == GameRulesNS::Difficulty::HARD) multiplier =
                        constants::DIFFICULTY_HARD_MULTIPLIER;
                    bool isPlayer = TagRegistry::getInstance().hasTag(
                        reg, selfEntity, constants::PLAYERTAG);
                    bool isMob = TagRegistry::getInstance().hasTag(
                        reg, selfEntity, constants::MOBTAG);
                    if (isPlayer) {
                        damage *= (2.0f - multiplier);
                    } else if (isMob) {
                        damage *= multiplier;
                    }
                }
                reg->addComponent<ecs::DamageIntentComponent>(selfEntity,
                    std::make_shared<ecs::DamageIntentComponent>(damage, otherEntity));
                auto cooldownComp =
                reg->getComponent<ecs::DamageCooldownComponent>(selfEntity);
                reg->getComponent<ecs::DamageCooldownComponent>(selfEntity);
                if (cooldownComp) {
                    cooldownComp->setLastDamageTime(0.0f);
                }
            }
        });

    registerAction(constants::ADDLIFE_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)resourceManager;
            auto lifeCompSelf = reg->getComponent<ecs::HealthComponent>(selfEntity);
            auto lifeCompOther = reg->getComponent<ecs::HealthComponent>(otherEntity);

            if (lifeCompSelf && lifeCompOther) {
                lifeCompOther->setHealth(
                    lifeCompOther->getHealth() + lifeCompSelf->getHealth()
                );
                lifeCompSelf->setHealth(0);
            }
        });

    registerAction(constants::INTERACTION_CALL_SCRIPTING_ACTION,
        [](std::shared_ptr<ecs::Registry> reg,
            std::shared_ptr<ResourceManager> resourceManager,
            ecs::Entity selfEntity,
            ecs::Entity otherEntity
        ) {
            (void)resourceManager;
            if (reg->hasComponent<ecs::ScriptingComponent>(selfEntity)) {
                auto scriptingComp = reg->getComponent<ecs::ScriptingComponent>(selfEntity);
                if (scriptingComp) {
                    scriptingComp->getFunction
                        (constants::ONINTERACT_FUNCTION)(selfEntity, otherEntity);
                }
            }
        });
}
