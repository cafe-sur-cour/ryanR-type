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
#include "../../../common/components/temporary/SpawnIntentComponent.hpp"
#include "../../../common/constants.hpp"

namespace ecs {

MapGeneratorSystem::MapGeneratorSystem(unsigned int seed)
    : _seed(seed), _rng(seed), _lastGeneratedX(0.0f),
    _generationStep(50.0f), _startGenerationX(500.0f),
    _waveTimer(0.0f), _waveInterval(5.0f),
    _powerUpTimer(0.0f), _powerUpInterval(3.0f) {
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
    _waveTimer += deltaTime;
    _powerUpTimer += deltaTime;

    auto gameZoneView = registry->view<GameZoneComponent>();

    auto gameZoneEntity = *gameZoneView.begin();
    auto gameZoneTransform = registry->getComponent<TransformComponent>(gameZoneEntity);
    auto gameZonePosition = gameZoneTransform->getPosition();

    float currentX = gameZonePosition.getX();

    while (_lastGeneratedX + _generationStep < currentX + constants::MAX_WIDTH * 2) {
        generateObstaclesAt(_lastGeneratedX + _generationStep, resourceManager, registry);
        _lastGeneratedX += _generationStep;
    }

    if (_waveTimer >= _waveInterval) {
        _waveTimer = 0.0f;
        std::uniform_real_distribution<float> probDist(0.0f, 1.0f);
        if (probDist(_rng) < 0.8f) {
            generateRandomWave(resourceManager, registry, currentX);
        }
    }

    if (_powerUpTimer >= _powerUpInterval) {
        _powerUpTimer = 0.0f;
        std::uniform_real_distribution<float> probDist(0.0f, 1.0f);
        if (probDist(_rng) < 0.9f) {
            generateRandomPowerUp(resourceManager, registry, currentX);
        }
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

    float floorNoise3 = noise(x * 0.1f + 400.0f);
    float ceilingNoise3 = noise(x * 0.1f + 500.0f);
    float floorY3 = floorNoise3 * 450.0f;
    float ceilingY3 = floorY3 + minPassageHeight + ceilingNoise3 * 400.0f;
    ceilingY3 = (std::min)(ceilingY3, 1000.0f);

    for (int row = 0; row < numRows; ++row) {
        float y = static_cast<float>(row) * obstacleSize - obstacleSize / 2.0f;

        if ((y < floorY || y > ceilingY) && (y < floorY2 || y > ceilingY2) &&
            (y < floorY3 || y > ceilingY3)) {
            ecs::Entity obstacle = prefabManager->createEntityFromPrefab(
                constants::OBSTACLE_1, registry, ecs::EntityCreationContext::forServer());
            auto transform = registry->getComponent<ecs::TransformComponent>(obstacle);
            if (transform) {
                transform->setPosition(math::Vector2f(x, y));
            }
        }
    }
}

void MapGeneratorSystem::generateRandomWave(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float currentX
) {
    (void) resourceManager;

    std::uniform_int_distribution<int> numDist(3, 6);
    int numEnemies = numDist(_rng);

    for (int i = 0; i < numEnemies; ++i) {
        std::uniform_int_distribution<int> prefabDist(0, 1);
        std::string prefabName = (prefabDist(_rng) == 0) ?
            constants::ENEMY_1 : constants::ENEMY_2;

        std::uniform_real_distribution<float> xDist(
            constants::MAX_WIDTH - 500, constants::MAX_WIDTH);
        std::uniform_real_distribution<float> yDist(
            50.0f, constants::MAX_HEIGHT - 50.0f);
        math::Vector2f relativePosition(xDist(_rng), yDist(_rng));

        auto intentEntity = registry->createEntity();
        registry->addComponent<SpawnIntentComponent>(
            intentEntity,
            std::make_shared<SpawnIntentComponent>(
                prefabName,
                relativePosition,
                ecs::EntityCreationContext::forServer(),
                currentX - 1.0f
            )
        );
    }
}

void MapGeneratorSystem::generateRandomPowerUp(
    std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry,
    float currentX
) {
    (void) resourceManager;

    std::uniform_int_distribution<int> typeDist(0, 3);
    std::string prefabName;

    int powerUpType = typeDist(_rng);
    switch (powerUpType) {
        case 0:
            prefabName = constants::POWERUP_ADD_LIFE;
            break;
        case 1:
            prefabName = constants::POWERUP_FLYING_FORCE;
            break;
        case 2:
            prefabName = constants::POWERUP_SPEED;
            break;
        case 3:
            prefabName = constants::POWERUP_CHARGE_TIME;
            break;
        default:
            prefabName = constants::POWERUP_ADD_LIFE;
            break;
    }

    std::uniform_real_distribution<float> xDist(
        constants::MAX_WIDTH * 0.25f, constants::MAX_WIDTH * 0.5f);
    std::uniform_real_distribution<float> yDist(
        100.0f, constants::MAX_HEIGHT - 100.0f);
    math::Vector2f relativePosition(xDist(_rng), yDist(_rng));

    auto intentEntity = registry->createEntity();
    registry->addComponent<SpawnIntentComponent>(
        intentEntity,
        std::make_shared<SpawnIntentComponent>(
            prefabName,
            relativePosition,
            ecs::EntityCreationContext::forServer(),
            currentX - 1.0f
        )
    );
}

}
