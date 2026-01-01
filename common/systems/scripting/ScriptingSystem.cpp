/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingSystem
*/

#include "ScriptingSystem.hpp"
#include <iostream>
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
    lua.set_function("print", [](const std::string& msg) {
        std::cout << "[Lua] " << msg << std::endl;
    });

    lua.set_function("createMoveIntent", [this](Entity e, float x, float y) {
        auto intent = std::make_shared<ecs::MovementIntentComponent>(math::Vector2f(x, y), true);
        registry->addComponent<ecs::MovementIntentComponent>(e, intent);
    });

    lua.set_function("getEntityPosition", [this](Entity e) -> std::tuple<float, float> {
        if (registry->hasComponent<TransformComponent>(e)) {
            auto transform = registry->getComponent<TransformComponent>(e);
            auto pos = transform->getPosition();
            return {pos.getX(), pos.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function("getPlayerPosition", [this]() -> std::tuple<float, float> {
        auto view = registry->view<PlayerTag, TransformComponent>();
        for (auto entityId : view) {
            auto transform = registry->getComponent<TransformComponent>(entityId);
            auto pos = transform->getPosition();
            return {pos.getX(), pos.getY()};
        }
        return {0.0f, 0.0f};
    });

    lua.set_function("getEntitySpeed", [this](Entity e) -> float {
        if (registry->hasComponent<SpeedComponent>(e)) {
            auto speedComp = registry->getComponent<SpeedComponent>(e);
            return speedComp->getSpeed();
        }
        return 0.0f;
    });

    lua.set_function("createShootIntent", [this](Entity e, float angleDegrees) {
        auto intent = std::make_shared<ecs::ShootIntentComponent>(angleDegrees);
        registry->addComponent<ecs::ShootIntentComponent>(e, intent);
    });
}

void ScriptingSystem::callInitForEntity(Entity entity, std::shared_ptr<ecs::ScriptingComponent> comp) {
    if (comp->hasFunction("init") && !comp->isInitialized()) {
        sol::protected_function initFunc(comp->getFunction("init"));
        sol::protected_function_result result = initFunc(entity);
        if (!result.valid()) {
            sol::error err = result;
            std::cerr << "Init error for entity " << entity << ": " << err.what() << std::endl;
        }
        comp->setInitialized(true);
    }
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

        callInitForEntity(entityId, scriptingComp);

        if (scriptingComp->hasFunction("update")) {
            sol::protected_function updateFunc(scriptingComp->getFunction("update"));
            sol::protected_function_result result = updateFunc(entityId, deltaTime);
            if (!result.valid()) {
                sol::error err = result;
            }
        }
    }
}

}  // namespace ecs
