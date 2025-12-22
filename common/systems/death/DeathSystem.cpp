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
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../components/permanent/OwnerComponent.hpp"
#include "../../components/permanent/ScoreValueComponent.hpp"
#include "../../components/permanent/ScoreComponent.hpp"
#include "../../components/temporary/ScoreIntentComponent.hpp"
#include "../../types/Vector2f.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../constants.hpp"
#include "../../ECS/entity/EntityCreationContext.hpp"
#include "../../../client/components/temporary/SoundIntentComponent.hpp"
#include "../SystemNames.hpp"

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
                auto ownerComp = registry->getComponent<ecs::OwnerComponent>(source);
                if (ownerComp) {
                    ecs::Entity owner = ownerComp->getOwner();
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

        registry->destroyEntity(entityId);
    }
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::DeathSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::DEATH_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
