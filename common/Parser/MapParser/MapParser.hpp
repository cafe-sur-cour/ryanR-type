/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapParser
*/

#ifndef MAPPARSER_HPP_
#define MAPPARSER_HPP_

#include <string>
#include <memory>
#include <nlohmann/json.hpp>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../constants.hpp"
#include "../../types/Vector2f.hpp"

class MapParser {
    public:
        MapParser(std::shared_ptr<EntityPrefabManager> prefabManager,
            std::shared_ptr<ecs::Registry> registry);
        ~MapParser();

        void parseMapFromFile(const std::string& filePath);
        void parseMap(const nlohmann::json& mapJson);
    protected:
    private:
        std::shared_ptr<EntityPrefabManager> _prefabManager;
        std::shared_ptr<ecs::Registry> _registry;
        void createBackgroundEntity(const std::string& entityName);
        void parseMapGrid(const nlohmann::json& legend, const nlohmann::json& mapGrid,
            float tileWidth, float tileHeight);
        void parseWaves(const nlohmann::json& waves, float tileWidth);
        ecs::Entity createEntityFromPrefab(const std::string& prefabName,
            float x, float y);
};

#endif /* !MAPPARSER_HPP_ */
