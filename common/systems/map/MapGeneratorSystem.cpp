/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapGeneratorSystem
*/

#include "MapGeneratorSystem.hpp"
#include <memory>
#include <string>
#include <algorithm>
#include <ctime>
#include <chrono>
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/components/permanent/GameZoneComponent.hpp"
#include "../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../common/ECS/entity/Entity.hpp"
#include "../../../common/constants.hpp"

namespace ecs {

MapGeneratorSystem::MapGeneratorSystem(unsigned int seed)
    : _seed(seed), _rng(seed), _lastGeneratedX(0.0f),
    _generationStep(50.0f), _startGenerationX(500.0f) {
    _lastGeneratedX = _startGenerationX - _generationStep;
    auto now = std::chrono::high_resolution_clock::now();
    _seed = static_cast<unsigned int>(now.time_since_epoch().count());
    _rng.seed(_seed);
}

void MapGeneratorSystem::update(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float deltaTime
) {
    (void) deltaTime;

    auto gameZoneView = registry->view<GameZoneComponent>();

    auto gameZoneEntity = *gameZoneView.begin();
    auto gameZoneTransform = registry->getComponent<TransformComponent>(gameZoneEntity);
    auto gameZonePosition = gameZoneTransform->getPosition();

    float currentX = gameZonePosition.getX();

    while (_lastGeneratedX + _generationStep < currentX + constants::MAX_WIDTH * 2) {
        generateObstaclesAt(_lastGeneratedX + _generationStep, resourceManager, registry);
        _lastGeneratedX += _generationStep;
    }
}

float MapGeneratorSystem::noise(float x) {
    float seedOffset = static_cast<float>(_seed) * 0.001f;
    return (sinf((x + seedOffset) * 0.01f) + cosf((x + seedOffset) * 0.007f) +
        sinf((x + seedOffset) * 0.03f) *
            0.3f + cosf((x + seedOffset) * 0.05f) * 0.2f) * 0.4f + 0.5f;
}

void MapGeneratorSystem::generateObstaclesAt(
    float x,
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry
) {
    auto prefabManager = resourceManager->get<EntityPrefabManager>();

    const float screenHeight = 1080.0f;
    const float obstacleSize = 40.0f;
    const int numRows = static_cast<int>(screenHeight / obstacleSize);
    const float minPassageHeight = 300.0f;

    float floorNoise = noise(x * 0.1f);
    float ceilingNoise = noise(x * 0.1f + 100.0f);

    float floorY = floorNoise * 450.0f;
    float ceilingY = floorY + minPassageHeight + ceilingNoise * 400.0f;
    ceilingY = (std::min)(ceilingY, 1000.0f);

    float floorNoise2 = noise(x * 0.1f + 200.0f);
    float ceilingNoise2 = noise(x * 0.1f + 300.0f);
    float floorY2 = floorNoise2 * 450.0f;
    float ceilingY2 = floorY2 + minPassageHeight + ceilingNoise2 * 400.0f;
    ceilingY2 = (std::min)(ceilingY2, 1000.0f);

    for (int row = 0; row < numRows; ++row) {
        float y = static_cast<float>(row) * obstacleSize - obstacleSize / 2.0f;

        if ((y < floorY || y > ceilingY) && (y < floorY2 || y > ceilingY2)) {
            ecs::Entity obstacle = prefabManager->createEntityFromPrefab(
                constants::OBSTACLE_1, registry, ecs::EntityCreationContext::forServer());
            auto transform = registry->getComponent<ecs::TransformComponent>(obstacle);
            if (transform) {
                transform->setPosition(math::Vector2f(x, y));
            }
        }
    }
}

}
