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
#include <algorithm>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../ECS/view/View.hpp"

namespace ecs {

ScriptingSystem::ScriptingSystem() : lua(), registry(nullptr), resourceManager(nullptr) {
    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::table);
    bindAPI();
}

void ScriptingSystem::update(
    std::shared_ptr<ResourceManager> rm,
    std::shared_ptr<Registry> reg,
    float deltaTime
) {
    (void) deltaTime;
    if (!reg)
        return;

    if (!this->registry)
        this->registry = reg;

    if (!this->resourceManager)
        this->resourceManager = rm;

    View<ecs::ScriptingComponent> scriptingView = reg->view<ecs::ScriptingComponent>();
    for (auto entityId : scriptingView) {
        auto scriptingComp = reg->getComponent<ecs::ScriptingComponent>(entityId);

        if (!scriptingComp)
            continue;

        if (!scriptingComp->isInitialized()) {
            scriptingComp->init(this->lua, entityId);
        }

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
