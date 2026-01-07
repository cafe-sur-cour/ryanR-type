/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorObstacles
*/

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include "LevelEditorState.hpp"
#include "../../../../constants.hpp"

namespace gsm {

void LevelEditorState::parseObstacles() {
    _obstaclesByName.clear();

    if (!_levelData.contains(constants::OBSTACLES_FIELD) ||
        !_levelData[constants::OBSTACLES_FIELD].is_array()) {
        return;
    }

    const auto& obstacles = _levelData[constants::OBSTACLES_FIELD];

    for (const auto& obstacle : obstacles) {
        if (!obstacle.contains(constants::OBSTACLE_NAME_FIELD)) {
            continue;
        }
        std::string obstacleName =
            obstacle[constants::OBSTACLE_NAME_FIELD].get<std::string>();

        if (_obstaclesByName.find(obstacleName) == _obstaclesByName.end()) {
            _obstaclesByName[obstacleName] = ObstacleGroup();
        }
        ObstacleGroup& group = _obstaclesByName[obstacleName];

        if (!obstacle.contains(constants::OBSTACLE_POSITIONS_FIELD) ||
            !obstacle[constants::OBSTACLE_POSITIONS_FIELD].is_array()) {
            continue;
        }

        const auto& positions = obstacle[constants::OBSTACLE_POSITIONS_FIELD];

        for (const auto& position : positions) {
            if (!position.contains(constants::OBSTACLE_TYPE_FIELD)) {
                continue;
            }

            std::string type = position[constants::OBSTACLE_TYPE_FIELD].get<std::string>();
            if (type == constants::OBSTACLE_HORIZONTAL_LINE_TYPE) {
                if (!position.contains(constants::OBSTACLE_FROMX_FIELD) ||
                    !position.contains(constants::OBSTACLE_POSY_FIELD) ||
                    !position.contains(constants::OBSTACLE_COUNT_FIELD)) {
                    continue;
                }

                HorizontalLineObstacle hLine;
                hLine.fromX = position[constants::OBSTACLE_FROMX_FIELD].get<float>();
                hLine.posY = position[constants::OBSTACLE_POSY_FIELD].get<float>();
                hLine.count = position[constants::OBSTACLE_COUNT_FIELD].get<int>();

                group.horizontalLines.push_back(hLine);
            } else if (type == constants::OBSTACLE_VERTICAL_LINE_TYPE) {
                if (!position.contains(constants::OBSTACLE_FROMY_FIELD) ||
                    !position.contains(constants::OBSTACLE_POSX_FIELD) ||
                    !position.contains(constants::OBSTACLE_COUNT_FIELD)) {
                    continue;
                }

                VerticalLineObstacle vLine;
                vLine.fromY = position[constants::OBSTACLE_FROMY_FIELD].get<float>();
                vLine.posX = position[constants::OBSTACLE_POSX_FIELD].get<float>();
                vLine.count = position[constants::OBSTACLE_COUNT_FIELD].get<int>();

                group.verticalLines.push_back(vLine);
            } else if (type == constants::OBSTACLE_UNIQUE_TYPE) {
                if (!position.contains(constants::OBSTACLE_POSX_FIELD) ||
                    !position.contains(constants::OBSTACLE_POSY_FIELD)) {
                    continue;
                }

                UniqueObstacle unique;
                unique.posX = position[constants::OBSTACLE_POSX_FIELD].get<float>();
                unique.posY = position[constants::OBSTACLE_POSY_FIELD].get<float>();

                group.uniques.push_back(unique);
            }
        }
    }
}

void LevelEditorState::renderAllObstacles(
    float levelX,
    float levelY,
    float canvasLeft,
    float canvasRight,
    float canvasTop,
    float canvasBottom
) {
    for (const auto& [prefabName, obstacleGroup] : _obstaclesByName) {
        std::string prefabPath =
            constants::OBSTACLES_DIRECTORY + prefabName + constants::LEVEL_FILE_EXTENSION;
        LevelPreviewSprite spriteData = extractSpriteDataFromPrefab(prefabPath);

        if (spriteData.texturePath.empty()) {
            continue;
        }

        for (const auto& hLine : obstacleGroup.horizontalLines) {
            for (int i = 0; i < hLine.count; ++i) {
                float worldX = hLine.fromX + (i * spriteData.width);
                float worldY = hLine.posY;

                float screenX = levelX + (worldX * _viewportZoom);
                float screenY = levelY + (worldY * _viewportZoom);

                renderSpriteInLevelPreview(
                    spriteData, screenX, screenY, canvasLeft,
                    canvasRight, canvasTop, canvasBottom);
            }
        }

        for (const auto& vLine : obstacleGroup.verticalLines) {
            for (int i = 0; i < vLine.count; ++i) {
                float worldX = vLine.posX;
                float worldY = vLine.fromY + (i * spriteData.height);

                float screenX = levelX + (worldX * _viewportZoom);
                float screenY = levelY + (worldY * _viewportZoom);

                renderSpriteInLevelPreview(
                    spriteData, screenX, screenY, canvasLeft,
                    canvasRight, canvasTop, canvasBottom);
            }
        }

        for (const auto& unique : obstacleGroup.uniques) {
            float screenX = levelX + (unique.posX * _viewportZoom);
            float screenY = levelY + (unique.posY * _viewportZoom);

            renderSpriteInLevelPreview(
                spriteData, screenX, screenY, canvasLeft,
                canvasRight, canvasTop, canvasBottom);
        }
    }
}

}  // namespace gsm
