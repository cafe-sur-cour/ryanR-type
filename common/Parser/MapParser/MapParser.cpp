/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapParser
*/

#include "MapParser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <string>
#include <memory>
#include <vector>
#include "../../Error/ParserError.hpp"
#include "../../constants.hpp"
#include "../../debug.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/NetworkIdComponent.hpp"
#include "../../types/Vector2f.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../../client/components/rendering/MusicComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/tags/GameEndTag.hpp"
#include "../../components/tags/GameZoneColliderTag.hpp"
#include "../../components/temporary/SpawnIntentComponent.hpp"
#include "../../ECS/entity/factory/EntityFactory.hpp"

MapParser::MapParser(std::shared_ptr<EntityPrefabManager> prefabManager,
    std::shared_ptr<ecs::Registry> registry)
    : _prefabManager(prefabManager), _registry(registry),
      _creationContext(ecs::EntityCreationContext::forServer()) {
    this->_mapJson = nullptr;
    if (!_prefabManager)
        throw err::ParserError("PrefabManager cannot be null", err::ParserError::UNKNOWN);
    if (!_registry)
        throw err::ParserError("Registry cannot be null", err::ParserError::UNKNOWN);
}

MapParser::~MapParser() {
}

void MapParser::parseMapFromFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open())
        throw err::ParserError("Cannot open map file: " +
            filePath, err::ParserError::FILE_NOT_FOUND);

    nlohmann::json mapJson;
    try {
        file >> mapJson;
    } catch (const nlohmann::detail::exception& e) {
        throw err::ParserError("Invalid JSON format in map file: " +
            filePath + " (" + e.what() + ")",
            err::ParserError::INVALID_FORMAT);
    }
    if (!this->_mapJson.is_null())
        this->_mapJson.clear();
    this->_mapJson = mapJson;
    parseMap(mapJson);
}

void MapParser::parseMap(const nlohmann::json &mapJson) {
    if (mapJson.contains(constants::BACKGROUND_FIELD))
        createBackgroundEntity(mapJson[constants::BACKGROUND_FIELD].get<std::string>());

    if (mapJson.contains(constants::MUSIC_FIELD))
        createMusicEntity(mapJson[constants::MUSIC_FIELD].get<std::string>());

    if (mapJson.contains(constants::BACKGROUND_SCROLL_SPEED_FIELD))
        createGameZoneEntity(mapJson[constants::BACKGROUND_SCROLL_SPEED_FIELD]);

    if (mapJson.contains(constants::MAP_LENGTH_FIELD))
        createGameEndEntity(mapJson[constants::MAP_LENGTH_FIELD]);

    if (mapJson.contains(constants::POWERUPS_FIELD))
        parsePowerUps(mapJson[constants::POWERUPS_FIELD]);

    if (mapJson.contains(constants::OBSTACLES_FIELD))
        parseObstacles(mapJson[constants::OBSTACLES_FIELD]);

    if (mapJson.contains(constants::WAVES_FIELD))
        parseWaves(mapJson[constants::WAVES_FIELD]);
}

void MapParser::generateMapEntities() {
    if (_mapJson.is_null()) {
        std::cout << "[MapParser] No map data available to generate entities" << std::endl;
        return;
    }
    if (_creationContext.origin == ecs::EntityCreationOrigin::CLIENT_LOCAL) {
        std::cout << "[MapParser] Skipping entity creation for client" << std::endl;
        return;
    }
    parseMap(_mapJson);
}

nlohmann::json MapParser::getMapJson() const {
    return _mapJson;
}

void MapParser::setMapJson(const nlohmann::json& mapJson) {
    _mapJson = mapJson;
}


void MapParser::createBackgroundEntity(const std::string& entityName) {
    if (entityName.empty())
        return;

    if (_prefabManager->hasPrefab(entityName)) {
        try {
            createEntityFromPrefab(entityName,
                constants::BACKGROUND_POSITION.getX(),
                constants::BACKGROUND_POSITION.getY());
        } catch (const std::exception& e) {
            std::cerr << "Error creating background entity: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Warning: 'background' prefab not found" << std::endl;
    }
}

void MapParser::createGameZoneEntity(float scrollSpeed) {
    ecs::Entity gameZoneEntity = _prefabManager->createEntityFromPrefab(
        constants::GAME_ZONE_PREFAB, _registry, _creationContext);

    if (_registry->hasComponent<ecs::VelocityComponent>(gameZoneEntity)) {
        auto velocityComp = _registry->getComponent<ecs::VelocityComponent>(gameZoneEntity);
        if (velocityComp) {
            velocityComp->setVelocity(math::Vector2f(scrollSpeed, 0.0f));
        }
    }
}

void MapParser::createGameEndEntity(float mapLength) {
    float x = mapLength;
    ecs::Entity gameEndEntity = _registry->createEntity();

    _registry->addComponent(gameEndEntity, std::make_shared<ecs::GameEndTag>());
    _registry->addComponent(gameEndEntity, std::make_shared<ecs::TransformComponent>
        (math::Vector2f(x, 0.0f)));
    _registry->addComponent(gameEndEntity, std::make_shared<ecs::ColliderComponent>(
        math::Vector2f(0.0f, 0.0f), math::Vector2f(10.0f, constants::MAX_HEIGHT),
        ecs::CollisionType::Trigger));
}

void MapParser::parsePowerUps(const nlohmann::json &powerUps) {
    if (!powerUps.is_array()) {
        std::cerr << "Error: powerUps is not a valid array" << std::endl;
        return;
    }

    for (size_t index = 0; index < powerUps.size(); ++index) {
        const auto &powerUp = powerUps[index];

        if (!powerUp.contains(constants::NAME_FIELD) ||
            !powerUp.contains(constants::POSITION_FIELD)) {
            std::cerr << "Warning: PowerUp " << index <<
                ": missing required fields (name/position), skipping" << std::endl;
            continue;
        }

        const std::string &prefabName = powerUp[constants::NAME_FIELD].get<std::string>();
        const auto &position = powerUp[constants::POSITION_FIELD];

        if (!position.contains(constants::POSX_FIELD) || !position.contains(constants::POSY_FIELD)) {
            std::cerr << "Warning: PowerUp " << index <<
                ": position missing posX or posY, skipping" << std::endl;
            continue;
        }

        float posX = position[constants::POSX_FIELD].get<float>();
        float posY = position[constants::POSY_FIELD].get<float>();

        if (_prefabManager->hasPrefab(prefabName)) {
            try {
                createEntityFromPrefab(prefabName, posX, posY);
            } catch (const std::exception& e) {
                std::cerr << "Error creating power-up entity '" << prefabName << "': " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Warning: Power-up prefab '" << prefabName << "' not found" << std::endl;
        }
    }
}

void MapParser::parseObstacles(const nlohmann::json &obstacles) {
    if (!obstacles.is_array()) {
        std::cerr << "Error: obstacles is not a valid array" << std::endl;
        return;
    }

    for (size_t index = 0; index < obstacles.size(); ++index) {
        const auto &obstacle = obstacles[index];

        if (
            !obstacle.contains(constants::NAME_FIELD) ||
            !obstacle.contains(constants::POSITIONS_FIELD)
        ) {
            std::cerr << "Warning: Obstacle " << index <<
                ": missing required fields (name/positions), skipping" << std::endl;
            continue;
        }

        const std::string &prefabName = obstacle[constants::NAME_FIELD].get<std::string>();

        if (!obstacle[constants::POSITIONS_FIELD].is_array()) {
            std::cerr << "Warning: Obstacle " << index <<
                " positions is not an array, skipping" << std::endl;
            continue;
        }

        for (const auto &position : obstacle[constants::POSITIONS_FIELD]) {
            if (
                !position.contains(constants::TYPE_FIELD)
            ) {
                std::cerr << "Warning: position in obstacle " << index
                    << " missing required fields (type/count)"
                    << ", skipping" << std::endl;
                continue;
            }

            const std::string &type = position[constants::TYPE_FIELD];

            if (type == constants::HORIZONTAL_LINE_TYPE) {
                if (
                    !position.contains(constants::FROMX_FIELD) ||
                    !position.contains(constants::POSY_FIELD) ||
                    !position.contains(constants::COUNT_FIELD)
                ) {
                    std::cerr << "Warning: position in obstacle " << index
                        << " missing required fields for type " << type
                        << "(fromX/posY), skipping" << std::endl;
                    continue;
                }

                int count = position[constants::COUNT_FIELD];

                float fromX = position[constants::FROMX_FIELD];
                float posY = position[constants::POSY_FIELD];

                for (int i = 0; i < count; ++i) {
                    auto entity = _prefabManager->createEntityFromPrefab(
                        prefabName,
                        _registry,
                        _creationContext
                    );
                    if (
                        !_registry->hasComponent<ecs::TransformComponent>(entity) ||
                        !_registry->hasComponent<ecs::ColliderComponent>(entity)
                    ) {
                        continue;
                    }
                    auto transComp = _registry->getComponent<ecs::TransformComponent>(entity);
                    auto colComp = _registry->getComponent<ecs::ColliderComponent>(entity);

                    auto rect = colComp->getScaledHitbox(
                        transComp->getPosition(),
                        transComp->getScale()
                    );
                    float width = rect.getWidth();

                    transComp->setPosition(
                        math::Vector2f(fromX + static_cast<float>(i) * width, posY)
                    );
                }
            }
            if (type == constants::VERTICAL_LINE_TYPE) {
                if (
                    !position.contains(constants::FROMY_FIELD) ||
                    !position.contains(constants::POSX_FIELD) ||
                    !position.contains(constants::COUNT_FIELD)
                ) {
                    std::cerr << "Warning: position in obstacle " << index
                        << " missing required fields for type " << type
                        << "(fromY/posX), skipping" << std::endl;
                    continue;
                }

                int count = position[constants::COUNT_FIELD];

                float fromY = position[constants::FROMY_FIELD];
                float posX = position[constants::POSX_FIELD];

                for (int i = 0; i < count; ++i) {
                    auto entity = _prefabManager->createEntityFromPrefab(
                        prefabName,
                        _registry,
                        _creationContext
                    );
                    if (
                        !_registry->hasComponent<ecs::TransformComponent>(entity) ||
                        !_registry->hasComponent<ecs::ColliderComponent>(entity)
                    ) {
                        continue;
                    }
                    auto transComp = _registry->getComponent<ecs::TransformComponent>(entity);
                    auto colComp = _registry->getComponent<ecs::ColliderComponent>(entity);

                    auto rect = colComp->getScaledHitbox(
                        transComp->getPosition(),
                        transComp->getScale()
                    );
                    float height = rect.getHeight();

                    transComp->setPosition(
                        math::Vector2f(posX, fromY + static_cast<float>(i) * height)
                    );
                }
            }
            if (type == constants::UNIQUE_TYPE) {
                if (
                    !position.contains(constants::POSX_FIELD) ||
                    !position.contains(constants::POSY_FIELD)
                ) {
                    std::cerr << "Warning: position in obstacle " << index
                        << " missing required fields for type " << type
                        << "(posX/posY), skipping" << std::endl;
                    continue;
                }

                float posX = position[constants::POSX_FIELD];
                float posY = position[constants::POSY_FIELD];

                auto entity = _prefabManager->createEntityFromPrefab(
                    prefabName,
                    _registry,
                    _creationContext
                );
                if (!_registry->hasComponent<ecs::TransformComponent>(entity)) {
                    continue;
                }
                auto transComp = _registry->getComponent<ecs::TransformComponent>(entity);

                transComp->setPosition(
                    math::Vector2f(posX, posY)
                );
            }
        }
    }
}

void MapParser::parseWaves(const nlohmann::json& waves) {
    if (!waves.is_array()) {
        std::cerr << "Error: Waves is not a valid array" << std::endl;
        return;
    }

    for (size_t waveIndex = 0; waveIndex < waves.size(); ++waveIndex) {
        const auto& wave = waves[waveIndex];

        if (
            !wave.contains(constants::GAMEXTRIGGER_FIELD) ||
            !wave.contains(constants::ENEMIES_FIELD)
        ) {
            std::cerr << "Warning: Wave " << waveIndex << ": missing required fields (" <<
                constants::GAMEXTRIGGER_FIELD << "/" << constants::ENEMIES_FIELD <<
                "), skipping" << std::endl;
            continue;
        }

        float gameViewXTrigger = wave[constants::GAMEXTRIGGER_FIELD];

        if (!wave[constants::ENEMIES_FIELD].is_array()) {
            std::cerr << "Warning: Wave " << waveIndex <<
                " enemies is not an array, skipping" << std::endl;
            continue;
        }

        auto spawner = _registry->createEntity();

        for (const auto& enemyGroup : wave[constants::ENEMIES_FIELD]) {
            if (
                !enemyGroup.contains(constants::TYPE_FIELD) ||
                !enemyGroup.contains(constants::DISTRIBUTIONX_FIELD) ||
                !enemyGroup.contains(constants::DISTRIBUTIONY_FIELD) ||
                !enemyGroup.contains(constants::COUNT_FIELD)
            ) {
                std::cerr << "Warning: Enemy group in wave " << waveIndex
                    << " missing required fields (type/count/distributionX/distributionY)"
                    << ", skipping" << std::endl;
                continue;
            }

            if (!enemyGroup[constants::DISTRIBUTIONX_FIELD].is_object()) {
                std::cerr << "Warning: Wave " << waveIndex <<
                    ": distributionX is not an object, skipping" << std::endl;
                continue;
            }
            auto distributionX = enemyGroup[constants::DISTRIBUTIONX_FIELD];
            if (
                !distributionX.contains(constants::MIN_FIELD) ||
                !distributionX.contains(constants::MAX_FIELD) ||
                !distributionX.contains(constants::TYPE_FIELD)
            ) {
                std::cerr << "Warning: distributionX in wave " << waveIndex <<
                    " is missing a required field (" <<
                    constants::MIN_FIELD << "/" <<
                    constants::MAX_FIELD << "/" <<
                    constants::TYPE_FIELD << "), skipping" << std::endl;
                continue;
            }

            if (!enemyGroup[constants::DISTRIBUTIONY_FIELD].is_object()) {
                std::cerr << "Warning: Wave " << waveIndex <<
                    ": distributionY is not an object, skipping" << std::endl;
                continue;
            }
            auto distributionY = enemyGroup[constants::DISTRIBUTIONY_FIELD];
            if (
                !distributionY.contains(constants::MIN_FIELD) ||
                !distributionY.contains(constants::MAX_FIELD) ||
                !distributionY.contains(constants::TYPE_FIELD)
            ) {
                std::cerr << "Warning: distributionY in wave " << waveIndex <<
                    " is missing a required field (" <<
                    constants::MIN_FIELD << "/" <<
                    constants::MAX_FIELD << "/" <<
                    constants::TYPE_FIELD << "), skipping" << std::endl;
                continue;
            }

            std::string enemyType = enemyGroup[constants::TYPE_FIELD].get<std::string>();
            int count = enemyGroup[constants::COUNT_FIELD];

            if (count <= 0) {
                std::cerr << "Warning: Invalid enemy count (" << count <<
                    ") in wave " << waveIndex << std::endl;
                continue;
            }

            if (!_prefabManager->hasPrefab(enemyType)) {
                std::cerr << "Warning: Enemy prefab '" << enemyType
                    << "' not found in wave " << waveIndex << std::endl;
                continue;
            }

            const std::vector<float> xPositions = getPositionsFromDistrib(
                count,
                distributionX,
                constants::MAX_WIDTH
            );
            const std::vector<float> yPositions = getPositionsFromDistrib(
                count,
                distributionY,
                constants::MAX_HEIGHT
            );

            for (size_t i = 0; i < static_cast<size_t>(count); ++i) {
                try {
                    _registry->addComponent(
                        spawner,
                        std::make_shared<ecs::SpawnIntentComponent>(
                            enemyType,
                            math::Vector2f(xPositions[i], yPositions[i]),
                            _creationContext,
                            gameViewXTrigger
                        )
                    );
                } catch (const std::exception& e) {
                    std::cerr << "Error creating enemy '" << enemyType <<
                        "': " << e.what() << std::endl;
                }
            }
        }
    }
}

std::vector<float> MapParser::getPositionsFromDistrib(
    int count,
    const nlohmann::json &distribution,
    float limit
) {
    const std::string type = distribution[constants::TYPE_FIELD].get<std::string>();
    int min = distribution[constants::MIN_FIELD];
    int max = distribution[constants::MAX_FIELD];

    std::vector<float> values;

    double real_min = static_cast<double>(limit) * static_cast<double>(min) / 100.0;
    double real_max = static_cast<double>(limit) * static_cast<double>(max) / 100.0;

    if (type == constants::RANDOM_TYPE) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(real_min, real_max);

        for (int i = 0; i < count; ++i) {
            values.push_back(static_cast<float>(dis(gen)));
        }
    }
    if (type == constants::UNIFORM_TYPE) {
        double offset = (real_max - real_min) / static_cast<double>(count);

        for (int i = 0; i < count; ++i) {
            values.push_back(
                static_cast<float>(real_min) +
                static_cast<float>(i) * static_cast<float>(offset)
            );
        }
    }

    return values;
}

ecs::Entity MapParser::createEntityFromPrefab(
    const std::string& prefabName,
    float x,
    float y
) {
    ecs::Entity entity = _prefabManager->createEntityFromPrefab(
        prefabName, _registry, _creationContext
    );

    if (_registry->hasComponent<ecs::TransformComponent>(entity)) {
        auto comp = _registry->getComponent<ecs::TransformComponent>(entity);
        if (comp)
            comp->setPosition(math::Vector2f(x, y));
    }

    return entity;
}

void MapParser::createMusicEntity(const std::string& prefabName) {
    if (_prefabManager->hasPrefab(prefabName)) {
        try {
            ecs::EntityCreationContext musicContext =
                ecs::EntityCreationContext::forServer();
            _prefabManager->createEntityFromPrefab(prefabName, _registry, musicContext);
        } catch (const std::exception& e) {
            std::cerr << "Error creating music entity: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Warning: 'music' prefab not found" << std::endl;
    }
}

void MapParser::setCreationContext(const ecs::EntityCreationContext& context) {
    _creationContext = context;
}

ecs::EntityCreationContext MapParser::getCreationContext() const {
    return _creationContext;
}
