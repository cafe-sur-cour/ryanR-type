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
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/NetworkIdComponent.hpp"
#include "../../types/Vector2f.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
#include "../../../client/components/rendering/MusicComponent.hpp"
#include "../../components/permanent/GameZoneComponent.hpp"
#include "../../components/tags/GameZoneColliderTag.hpp"
#include "../../ECS/entity/factory/EntityFactory.hpp"

MapParser::MapParser(std::shared_ptr<EntityPrefabManager> prefabManager,
    std::shared_ptr<ecs::Registry> registry)
    : _prefabManager(prefabManager), _registry(registry),
      _creationContext(ecs::EntityCreationContext::forServer()) {
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
        createBackgroundEntity(mapJson[constants::BACKGROUND_FIELD]);

    if (mapJson.contains(constants::MUSIC_FIELD)) {
        std::string prefabName = mapJson[constants::MUSIC_FIELD].get<std::string>();
        createMusicEntity(prefabName);
    }
    if (mapJson.contains(constants::BACKGROUND_SCROLL_SPEED_FIELD))
        createGameZoneEntity(mapJson[constants::BACKGROUND_SCROLL_SPEED_FIELD]);

    float tileWidth = constants::TILE_SIZE.getX();
    float tileHeight = constants::TILE_SIZE.getY();

    if (mapJson.contains(constants::TILE_FIELD)) {
        tileWidth = mapJson[constants::TILE_FIELD].
            value(constants::WIDTH_FIELD, tileWidth);
        tileHeight = mapJson[constants::TILE_FIELD].
            value(constants::HEIGHT_FIELD, tileHeight);
    }

    if (mapJson.contains(constants::LEGEND_FIELD) && mapJson.contains(constants::MAP_FIELD))
        parseMapGrid(mapJson[constants::LEGEND_FIELD], mapJson[constants::MAP_FIELD],
            tileWidth, tileHeight);

    if (mapJson.contains(constants::WAVES_FIELD))
        parseWaves(mapJson[constants::WAVES_FIELD], tileWidth);
}


std::vector<std::uint64_t> MapParser::createPacketFromMap() {
    std::string jsonStr = this->_mapJson.dump();

    std::vector<std::uint64_t> data;
    for (size_t i = 0; i < jsonStr.size(); i += 8) {
        uint64_t value = 0;
        for (size_t j = 0; j < 8 && (i + j) < jsonStr.size(); ++j) {
            value |= static_cast<uint64_t>(static_cast<uint8_t>(jsonStr[i + j])) << (j * 8);
        }
        data.push_back(value);
    }
    return data;
}


void MapParser::parseMapFromPacket(std::vector<uint8_t> mapData) {
    std::string jsonStr(mapData.begin(), mapData.end());

    nlohmann::json mapJson;
    try {
        mapJson = nlohmann::json::parse(jsonStr);
    } catch (const nlohmann::detail::exception& e) {
        throw err::ParserError("Invalid JSON format in map data: "
            + std::string(e.what()),
            err::ParserError::INVALID_FORMAT);
    }
    if (!this->_mapJson.is_null())
        this->_mapJson.clear();
    this->_mapJson = mapJson;
    parseMap(mapJson);
}

void MapParser::generateMapEntities() {
    if (_mapJson.is_null()) {
        std::cout << "[MapParser] No map data available to generate entities" << std::endl;
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

void MapParser::parseMapGrid(const nlohmann::json& legend, const nlohmann::json& mapGrid,
    float tileWidth, float tileHeight) {
    if (!legend.is_object()) {
        std::cerr << "Error: Legend is not a valid object" << std::endl;
        return;
    }

    if (!mapGrid.is_array()) {
        std::cerr << "Error: Map grid is not a valid array" << std::endl;
        return;
    }

    for (size_t row = 0; row < mapGrid.size(); ++row) {
        if (!mapGrid[row].is_string()) {
            std::cerr << "Warning: Map row " << row <<
                " is not a string, skipping" << std::endl;
            continue;
        }

        std::string line = mapGrid[row].get<std::string>();
        std::istringstream iss(line);
        std::string token;
        size_t col = 0;

        while (iss >> token) {
            if (token == " ") {
                ++col;
                continue;
            }

            if (legend.contains(token)) {
                std::string prefabName = legend[token].get<std::string>();
                if (prefabName == constants::EMPTY_PREFAB) {
                    ++col;
                    continue;
                }

                if (_prefabManager->hasPrefab(prefabName)) {
                    float x = static_cast<float>(col) * tileWidth;
                    float y = static_cast<float>(row) * tileHeight;
                    try {
                        createEntityFromPrefab(prefabName, x, y);
                    } catch (const std::exception& e) {
                        std::cerr << "Error creating entity '" << prefabName << "': "
                        << e.what() << std::endl;
                    }
                } else {
                    std::cerr << "Warning: Prefab '" << prefabName
                        << "' not found for token '" << token
                        << "' at position (" << col << ", " << row << ")" << std::endl;
                }
            } else {
                std::cerr << "Error: Token '" << token
                    << "' not found in legend at position("
                    << col << ", " << row << ")" << std::endl;
            }
            ++col;
        }
    }
}

void MapParser::parseWaves(const nlohmann::json& waves, float tileWidth) {
    if (!waves.is_array()) {
        std::cerr << "Error: Waves is not a valid array" << std::endl;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disY(0.0, static_cast<double>(constants::MAX_HEIGHT));
    std::uniform_real_distribution<> disX(0.0, 1.0);

    for (size_t waveIndex = 0; waveIndex < waves.size(); ++waveIndex) {
        const auto& wave = waves[waveIndex];

        if (!wave.contains(constants::SPAWNLENGTH_FIELD) ||
            !wave.contains(constants::ENEMIES_FIELD)) {
            std::cerr << "Warning: Wave " << waveIndex
                << " missing required fields (spawnLength/enemies), skipping" << std::endl;
            continue;
        }

        int spawnLength = wave[constants::SPAWNLENGTH_FIELD].get<int>();
        int colIndex = wave.value(constants::POSX_FIELD, 0);

        if (!wave[constants::ENEMIES_FIELD].is_array()) {
            std::cerr << "Warning: Wave " << waveIndex <<
                " enemies is not an array, skipping" << std::endl;
            continue;
        }

        for (const auto& enemyGroup : wave[constants::ENEMIES_FIELD]) {
            if (!enemyGroup.contains(constants::TYPE_FIELD) ||
                !enemyGroup.contains(constants::COUNT_FIELD)) {
                std::cerr << "Warning: Enemy group in wave " << waveIndex
                    << " missing required fields (type/count), skipping" << std::endl;
                continue;
            }

            std::string enemyType = enemyGroup[constants::TYPE_FIELD].get<std::string>();
            int count = enemyGroup[constants::COUNT_FIELD].get<int>();

            if (count <= 0) {
                std::cerr << "Warning: Invalid enemy count (" << count <<
                    ") in wave " << waveIndex << std::endl;
                continue;
            }

            if (_prefabManager->hasPrefab(enemyType)) {
                for (int i = 0; i < count; ++i) {
                    float x = static_cast<float>(colIndex) * tileWidth +
                        static_cast<float>(disX(gen)) * static_cast<float>(spawnLength)
                        * tileWidth;
                    float y = static_cast<float>(disY(gen));
                    try {
                        createEntityFromPrefab(enemyType, x, y);
                    } catch (const std::exception& e) {
                        std::cerr << "Error creating enemy '" << enemyType <<
                            "': " << e.what() << std::endl;
                    }
                }
            } else {
                std::cerr << "Warning: Enemy prefab '" << enemyType
                    << "' not found in wave " << waveIndex << std::endl;
            }
        }
    }
}

ecs::Entity MapParser::createEntityFromPrefab(const std::string& prefabName,
    float x, float y) {
    ecs::Entity entity = _prefabManager->createEntityFromPrefab(
        prefabName, _registry, _creationContext);

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
                ecs::EntityCreationContext::forLocalClient();
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
