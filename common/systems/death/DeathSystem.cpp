/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeathSystem
*/

#include "DeathSystem.hpp"
#include <memory>
#include <exception>
#include <string>

#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/OwnerComponent.hpp"
#include "../../components/permanent/ScoreValueComponent.hpp"
#include "../../components/permanent/ScoreComponent.hpp"
#include "../../components/temporary/ScoreIntentComponent.hpp"
#include "../../components/permanent/CompositeEntityComponent.hpp"
#include "../../types/Vector2f.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../constants.hpp"
#include "../../ECS/entity/EntityCreationContext.hpp"
#include "../../../client/components/temporary/SoundIntentComponent.hpp"

// Disable sign-conversion warning for Sol2 include
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5321)
#endif
#include "../../components/permanent/ScriptingComponent.hpp"
#include <sol/sol.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

namespace ecs {

DeathSystem::DeathSystem() {
}

void DeathSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;
    (void) resourceManager;

    auto view = registry->view<DeathIntentComponent>();

    for (auto entityId : view) {
        auto deathIntent = registry->getComponent<DeathIntentComponent>(entityId);
        if (!deathIntent) continue;

        if (registry->hasComponent<ScoreValueComponent>(entityId)) {
            ecs::Entity source = deathIntent->getSource();
            if (source != 0) {
                ecs::Entity owner = 0;
                auto ownerComp = registry->getComponent<ecs::OwnerComponent>(source);
                if (ownerComp) {
                    owner = ownerComp->getOwner();
                } else {
                    auto parentComp =
                        registry->getComponent<ecs::CompositeEntityComponent>(source);
                    if (parentComp) {
                        owner = parentComp->getParentId();
                    }
                }
                if (owner != 0) {
                    auto scoreComp = registry->getComponent<ecs::ScoreComponent>(owner);
                    if (scoreComp) {
                        auto scoreValueComp =
                            registry->getComponent<ecs::ScoreValueComponent>(entityId);
                        if (scoreValueComp) {
                            int score = scoreValueComp->getScoreValue();
                            registry->addComponent(owner,
                                std::make_shared<ScoreIntentComponent>(score));
                        }
                    }
                }
            }
        }

        if (registry->hasComponent<ecs::ScriptingComponent>(entityId)) {
            auto scriptingComp = registry->getComponent<ecs::ScriptingComponent>(entityId);
            if (scriptingComp->hasFunction(constants::DEATH_FUNCTION)) {
                sol::protected_function deathFunc =
                    scriptingComp->getFunction(constants::DEATH_FUNCTION);
                sol::protected_function_result result = deathFunc(entityId);
                if (!result.valid()) {
                    sol::error err = result;
                    std::string what = err.what();
                }
            }
        }
        registry->destroyEntity(entityId);
    }
}

}  // namespace ecs
