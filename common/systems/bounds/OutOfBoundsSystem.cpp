/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OutOfBoundsSystem
*/

#include "OutOfBoundsSystem.hpp"
#include <memory>
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/temporary/DeathIntentComponent.hpp"
#include "../../components/tags/PlayerProjectileTag.hpp"
#include "../../components/tags/EnnemyProjectileTag.hpp"
#include "../../components/tags/MobTag.hpp"
#include "../../constants.hpp"
#include "../SystemNames.hpp"

namespace ecs {

OutOfBoundsSystem::OutOfBoundsSystem() : _margin(constants::OUT_OF_BOUNDS_MARGIN) {
}

void OutOfBoundsSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void)resourceManager;
    (void)deltaTime;

    math::FRect gameZoneBounds(0.0f, 0.0f, constants::MAX_WIDTH, constants::MAX_HEIGHT);
    float gameZoneX = 0.0f;

    auto gameZoneView = registry->view<GameZoneComponent, TransformComponent>();
    for (auto entityId : gameZoneView) {
        auto gameZone = registry->getComponent<GameZoneComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);
        if (gameZone && transform) {
            gameZoneBounds = gameZone->getZone();
            gameZoneX = transform->getPosition().getX();
            break;
        }
    }

    float leftBound = gameZoneX - _margin;
    float rightBound = gameZoneX + gameZoneBounds.getWidth() + _margin;
    float topBound = -_margin;
    float bottomBound = gameZoneBounds.getHeight() + _margin;

    auto view = registry->view<TransformComponent>();

    for (auto entityId : view) {
        bool shouldCheck = registry->hasComponent<PlayerProjectileTag>(entityId) ||
                          registry->hasComponent<EnnemyProjectileTag>(entityId) ||
                          registry->hasComponent<MobTag>(entityId);

        if (!shouldCheck)
            continue;
        if (registry->hasComponent<DeathIntentComponent>(entityId))
            continue;

        auto transform = registry->getComponent<TransformComponent>(entityId);
        if (!transform)
            continue;

        math::Vector2f pos = transform->getPosition();
        float x = pos.getX();
        float y = pos.getY();

        if (x < leftBound || x > rightBound || y < topBound || y > bottomBound) {
            registry->addComponent(entityId, std::make_shared<DeathIntentComponent>());
        }
    }
}

}  // namespace ecs

extern "C" ecs::ISystem* createSystem() {
    return new ecs::OutOfBoundsSystem();
}

extern "C" const char* getSystemName() {
    return ecs::systems::OUT_OF_BOUNDS_SYSTEM.c_str();
}

extern "C" void destroySystem(ecs::ISystem* system) {
    delete system;
}
