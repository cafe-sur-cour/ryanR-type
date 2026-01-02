/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingSystem
*/

#include "ScriptingSystem.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <sol/sol.hpp>
#include "../../components/permanent/ScriptingComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../ECS/view/View.hpp"
#include "../../components/temporary/MovementIntentComponent.hpp"
#include "../../components/temporary/ShootIntentComponent.hpp"


namespace ecs {

ScriptingSystem::ScriptingSystem() : lua(), registry(nullptr) {
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
    bindAPI();
}

void ScriptingSystem::bindAPI() {
    lua.set_function(constants::PRINT_FUNCTION, [](const std::string& msg) {
        std::cout << "[Lua] " << msg << std::endl;
    });

    lua.set_function(constants::CREATE_MOVE_INTENT_FUNCTION,
        [this](Entity e, float x, float y) {
        auto intent = std::make_shared<ecs::MovementIntentComponent>
            (math::Vector2f(x, y), true);
        registry->addComponent<ecs::MovementIntentComponent>(e, intent);
    });

    lua.set_function(constants::GET_ENTITY_POSITION_FUNCTION,
            [this](Entity e) -> std::tuple<float, float> {
        if (registry->hasComponent<TransformComponent>(e)) {
            auto transform = registry->getComponent<TransformComponent>(e);
            auto pos = transform->getPosition();
            return {pos.getX(), pos.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::GET_PLAYER_POSITION_FUNCTION,
            [this]() -> std::tuple<float, float> {
        auto view = registry->view<PlayerTag, TransformComponent>();
        for (auto entityId : view) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            auto pos = transform->getPosition();
            return {pos.getX(), pos.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function(constants::GET_ENTITY_SPEED_FUNCTION, [this](Entity e) -> float {
        if (registry->hasComponent<SpeedComponent>(e)) {
            auto speedComp = registry->getComponent<SpeedComponent>(e);
            return speedComp->getSpeed();
        }
        return 0.0f;
    });

    lua.set_function(constants::CREATE_SHOOT_INTENT_FUNCTION,
        [this](Entity e, float angleDegrees) {
        if (registry->hasComponent<ShootIntentComponent>(e))
            return;
        auto intent = std::make_shared<ecs::ShootIntentComponent>(angleDegrees);
        registry->addComponent<ecs::ShootIntentComponent>(e, intent);
    });
}

void ScriptingSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> reg,
    float deltaTime
) {
    (void) resourceManager;
    if (!reg)
        return;

    if (!this->registry)
        this->registry = reg;

    View<ecs::ScriptingComponent> scriptingView = reg->view<ecs::ScriptingComponent>();
    for (auto entityId : scriptingView) {
        auto scriptingComp = reg->getComponent<ecs::ScriptingComponent>(entityId);

        if (!scriptingComp)
            continue;

        if (!scriptingComp->isInitialized())
            scriptingComp->init(this->lua);

        if (scriptingComp->hasFunction(constants::UPDATE_FUNCTION)) {
            sol::protected_function updateFunc
                (scriptingComp->getFunction(constants::UPDATE_FUNCTION));
            sol::protected_function_result result = updateFunc(entityId, deltaTime);
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "[ScriptingSystem] Error in 'update' function for entity "
                    << entityId << ": " << err.what() << std::endl;
            }
        }
    }
}

}  // namespace ecs
