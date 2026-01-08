/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ServerForceInputSystem
*/

#include <memory>
#include <vector>
#include <iostream>
#include <sol/sol.hpp>
#include "ServerForceInputSystem.hpp"
#include "../../../common/InputMapping/InputAction.hpp"
#include "../../../common/components/tags/ControllableTag.hpp"
#include "../../../common/components/tags/PlayerTag.hpp"
#include "../../../common/InputMapping/IInputProvider.hpp"
#include "../../initResourcesManager/ServerInputProvider.hpp"
#include "../../../common/components/permanent/EntityPartsComponent.hpp"
#include "../../../common/components/permanent/CompositeEntityComponent.hpp"
#include "../../../common/components/permanent/ScriptingComponent.hpp"
#include "../../../common/components/tags/ForceTag.hpp"

namespace ecs {

ServerForceInputSystem::ServerForceInputSystem() {
}

void ServerForceInputSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime) {
    (void)deltaTime;

    if (!resourceManager->has<ecs::ServerInputProvider>()) {
        return;
    }

    auto serverInputProvider = resourceManager->get<ecs::ServerInputProvider>();

    auto view = registry->view<ControllableTag, PlayerTag>();

    for (auto entityId : view) {
        size_t clientID = static_cast<size_t>(entityId);
        float value = serverInputProvider->getActionAxis(InputAction::FORCE, clientID);
        if (value > 0.5f) {
            bool hasAttachedForce = false;
            if (registry->hasComponent<ecs::EntityPartsComponent>(entityId)) {
                auto partsComp = registry->getComponent<ecs::EntityPartsComponent>(entityId);
                for (auto partId : partsComp->partIds) {
                    if (registry->hasComponent<ecs::ScriptingComponent>(partId) &&
                        registry->hasComponent<ecs::ForceTag>(partId)) {
                        auto forceTag = registry->getComponent<ecs::ForceTag>(partId);
                        if (forceTag->getForceType() == constants::FORCE_TYPE) {
                            hasAttachedForce = true;
                            callActivateOrDeactivateForce(registry, partId, entityId);
                        }
                    }
                }
            }
            if (!hasAttachedForce) {
                auto viewcomp = registry->view<ecs::ScriptingComponent, ecs::ForceTag>();
                for (auto partId : viewcomp) {
                    if (registry->hasComponent<ecs::ForceTag>(partId)) {
                        auto forceTag = registry->getComponent<ecs::ForceTag>(partId);
                        if (forceTag->getForceType() != constants::FORCE_TYPE)
                            continue;
                    }
                    bool isFree = false;
                    if (registry->hasComponent<ecs::CompositeEntityComponent>(partId)) {
                        auto compositeComp =
                            registry->getComponent<ecs::CompositeEntityComponent>(partId);
                        if (compositeComp->getParentId() == 0) {
                            isFree = true;
                        }
                    } else {
                        isFree = true;
                    }
                    if (isFree) {
                        callActivateOrDeactivateForce(registry, partId, entityId);
                    }
                }
            }
            serverInputProvider->setAxisValue(InputAction::FORCE, 0.0f, clientID);
        }
    }
}

void ServerForceInputSystem::callActivateOrDeactivateForce(
    std::shared_ptr<Registry> registry,
    Entity partId,
    Entity entityId
) {
    auto scriptingComp = registry->getComponent<ecs::ScriptingComponent>(partId);
    if (scriptingComp) {
            if (scriptingComp->hasFunction(constants::ACTIVATE_OR_DEACTIVATE_FORCE_FUNCTION)) {
                scriptingComp->getFunction
                    (constants::ACTIVATE_OR_DEACTIVATE_FORCE_FUNCTION)(partId, entityId);
            }
        }
}

}  // namespace ecs
