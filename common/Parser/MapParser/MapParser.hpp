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
#include <vector>
#include <nlohmann/json.hpp>
#include "../../ECS/entity/registry/Registry.hpp"
#include "../../ECS/entity/EntityCreationContext.hpp"
#include "../../Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../constants.hpp"
#include "../../types/Vector2f.hpp"

class MapParser {
    public:
        MapParser(
            std::shared_ptr<EntityPrefabManager> prefabManager,
            std::shared_ptr<ecs::Registry> registry
        );
        ~MapParser();

        void parseMapFromFile(const std::string& filePath);
        void parseMap(const nlohmann::json& mapJson);

        void generateMapEntities();

        nlohmann::json getMapJson() const;
        void setMapJson(const nlohmann::json& mapJson);

        void setCreationContext(const ecs::EntityCreationContext& context);
        ecs::EntityCreationContext getCreationContext() const;

    private:
        std::shared_ptr<EntityPrefabManager> _prefabManager;
        std::shared_ptr<ecs::Registry> _registry;
        ecs::EntityCreationContext _creationContext;
        nlohmann::json _mapJson;

        void createBackgroundEntity(const std::string &entityName);
        void createMusicEntity(const std::string &prefabName);
        void createGameZoneEntity(float scrollSpeed);
        void createGameEndEntity(float mapLength);
        void createGameZoneStopEntity(float stopAtX);

        void parsePowerUps(const nlohmann::json &powerUps);
        void parseObstacles(const nlohmann::json &obstacles);
        void parseWaves(const nlohmann::json &waves);

        std::vector<float> getPositionsFromDistrib(
            int count,
            const nlohmann::json &distribution,
            float limit
        );
        ecs::Entity createEntityFromPrefab(
            const std::string &prefabName,
            float x, float y
        );
};

#endif /* !MAPPARSER_HPP_ */
