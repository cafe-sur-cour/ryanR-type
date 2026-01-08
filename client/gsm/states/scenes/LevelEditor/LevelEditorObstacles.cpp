/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorObstacles
*/

#include <fstream>
#include <string>
#include <utility>
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
    auto window = _resourceManager->get<gfx::IWindow>();
    gfx::color_t lightBlue = {100, 200, 255, 255};

    for (const auto& [prefabName, obstacleGroup] : _obstaclesByName) {
        std::string prefabPath =
            constants::OBSTACLES_DIRECTORY + "/" +
                prefabName + constants::LEVEL_FILE_EXTENSION;
        LevelPreviewSprite spriteData = extractSpriteDataFromPrefab(prefabPath);

        if (spriteData.texturePath.empty()) {
            continue;
        }

        float scaledWidth = spriteData.width * _viewportZoom;
        float scaledHeight = spriteData.height * _viewportZoom;

        for (size_t hIdx = 0; hIdx < obstacleGroup.horizontalLines.size(); ++hIdx) {
            const auto& hLine = obstacleGroup.horizontalLines[hIdx];
            for (int i = 0; i < hLine.count; ++i) {
                float worldX = hLine.fromX + (i * spriteData.width);
                float worldY = hLine.posY;

                float screenX = levelX + (worldX * _viewportZoom);
                float screenY = levelY + (worldY * _viewportZoom);

                renderSpriteInLevelPreview(
                    spriteData, screenX, screenY, canvasLeft,
                    canvasRight, canvasTop, canvasBottom);
            }

            if (_selectedObstacle.has_value() &&
                _selectedObstacle.value().prefabName == prefabName &&
                _selectedObstacle.value().type == "horizontal" &&
                _selectedObstacle.value().index == static_cast<int>(hIdx)) {
                float screenX = levelX + (hLine.fromX * _viewportZoom);
                float screenY = levelY + (hLine.posY * _viewportZoom);
                float totalWidth = hLine.count * scaledWidth;
                float totalHeight = scaledHeight;

                window->drawRectangleOutline(
                    lightBlue,
                    std::make_pair(static_cast<size_t>(
                        screenX), static_cast<size_t>(screenY)),
                    std::make_pair(static_cast<size_t>(
                        totalWidth), static_cast<size_t>(totalHeight)),
                    2
                );
            }
        }

        for (size_t vIdx = 0; vIdx < obstacleGroup.verticalLines.size(); ++vIdx) {
            const auto& vLine = obstacleGroup.verticalLines[vIdx];
            for (int i = 0; i < vLine.count; ++i) {
                float worldX = vLine.posX;
                float worldY = vLine.fromY + (i * spriteData.height);

                float screenX = levelX + (worldX * _viewportZoom);
                float screenY = levelY + (worldY * _viewportZoom);

                renderSpriteInLevelPreview(
                    spriteData, screenX, screenY, canvasLeft,
                    canvasRight, canvasTop, canvasBottom);
            }

            if (_selectedObstacle.has_value() &&
                _selectedObstacle.value().prefabName == prefabName &&
                _selectedObstacle.value().type == "vertical" &&
                _selectedObstacle.value().index == static_cast<int>(vIdx)) {
                float screenX = levelX + (vLine.posX * _viewportZoom);
                float screenY = levelY + (vLine.fromY * _viewportZoom);
                float totalWidth = scaledWidth;
                float totalHeight = vLine.count * scaledHeight;

                window->drawRectangleOutline(
                    lightBlue,
                    std::make_pair(static_cast<size_t>(
                        screenX), static_cast<size_t>(screenY)),
                    std::make_pair(static_cast<size_t>(
                        totalWidth), static_cast<size_t>(totalHeight)),
                    2
                );
            }
        }

        for (size_t uIdx = 0; uIdx < obstacleGroup.uniques.size(); ++uIdx) {
            const auto& unique = obstacleGroup.uniques[uIdx];
            float screenX = levelX + (unique.posX * _viewportZoom);
            float screenY = levelY + (unique.posY * _viewportZoom);

            renderSpriteInLevelPreview(
                spriteData, screenX, screenY, canvasLeft,
                canvasRight, canvasTop, canvasBottom);

            if (_selectedObstacle.has_value() &&
                _selectedObstacle.value().prefabName == prefabName &&
                _selectedObstacle.value().type == "unique" &&
                _selectedObstacle.value().index == static_cast<int>(uIdx)) {
                window->drawRectangleOutline(
                    lightBlue,
                    std::make_pair(static_cast<size_t>(
                        screenX), static_cast<size_t>(screenY)),
                    std::make_pair(static_cast<size_t>(
                        scaledWidth), static_cast<size_t>(scaledHeight)),
                    2
                );
            }
        }
    }
}

void LevelEditorState::saveObstacles() {
    nlohmann::json obstaclesArray = nlohmann::json::array();

    for (const auto& [prefabName, obstacleGroup] : _obstaclesByName) {
        nlohmann::json obstacleEntry;
        obstacleEntry[constants::OBSTACLE_NAME_FIELD] = prefabName;

        nlohmann::json positionsArray = nlohmann::json::array();

        for (const auto& hLine : obstacleGroup.horizontalLines) {
            nlohmann::json position;
            position[constants::OBSTACLE_TYPE_FIELD] =
                constants::OBSTACLE_HORIZONTAL_LINE_TYPE;
            position[constants::OBSTACLE_FROMX_FIELD] = hLine.fromX;
            position[constants::OBSTACLE_POSY_FIELD] = hLine.posY;
            position[constants::OBSTACLE_COUNT_FIELD] = hLine.count;
            positionsArray.push_back(position);
        }

        for (const auto& vLine : obstacleGroup.verticalLines) {
            nlohmann::json position;
            position[constants::OBSTACLE_TYPE_FIELD] =
                constants::OBSTACLE_VERTICAL_LINE_TYPE;
            position[constants::OBSTACLE_FROMY_FIELD] = vLine.fromY;
            position[constants::OBSTACLE_POSX_FIELD] = vLine.posX;
            position[constants::OBSTACLE_COUNT_FIELD] = vLine.count;
            positionsArray.push_back(position);
        }

        for (const auto& unique : obstacleGroup.uniques) {
            nlohmann::json position;
            position[constants::OBSTACLE_TYPE_FIELD] = constants::OBSTACLE_UNIQUE_TYPE;
            position[constants::OBSTACLE_POSX_FIELD] = unique.posX;
            position[constants::OBSTACLE_POSY_FIELD] = unique.posY;
            positionsArray.push_back(position);
        }

        obstacleEntry[constants::OBSTACLE_POSITIONS_FIELD] = positionsArray;
        obstaclesArray.push_back(obstacleEntry);
    }

    _levelData[constants::OBSTACLES_FIELD] = obstaclesArray;
}

std::optional<ObstacleSelection> LevelEditorState::getObstacleAtPosition(
    float mouseX, float mouseY, float levelX, float levelY
) {
    for (const auto& [prefabName, obstacleGroup] : _obstaclesByName) {
        std::string prefabPath =
            constants::OBSTACLES_DIRECTORY + "/" +
                prefabName + constants::LEVEL_FILE_EXTENSION;
        LevelPreviewSprite spriteData = extractSpriteDataFromPrefab(prefabPath);

        if (spriteData.texturePath.empty()) {
            continue;
        }

        float scaledWidth = spriteData.width * _viewportZoom;
        float scaledHeight = spriteData.height * _viewportZoom;

        for (size_t i = 0; i < obstacleGroup.uniques.size(); ++i) {
            const auto& unique = obstacleGroup.uniques[i];
            float screenX = levelX + (unique.posX * _viewportZoom);
            float screenY = levelY + (unique.posY * _viewportZoom);

            if (mouseX >= screenX && mouseX < screenX + scaledWidth &&
                mouseY >= screenY && mouseY < screenY + scaledHeight) {
                return ObstacleSelection{prefabName, "unique", static_cast<int>(i)};
            }
        }

        for (size_t i = 0; i < obstacleGroup.horizontalLines.size(); ++i) {
            const auto& hLine = obstacleGroup.horizontalLines[i];
            for (int j = 0; j < hLine.count; ++j) {
                float worldX = hLine.fromX + (j * spriteData.width);
                float worldY = hLine.posY;
                float screenX = levelX + (worldX * _viewportZoom);
                float screenY = levelY + (worldY * _viewportZoom);

                if (mouseX >= screenX && mouseX < screenX + scaledWidth &&
                    mouseY >= screenY && mouseY < screenY + scaledHeight) {
                    return ObstacleSelection{prefabName, "horizontal", static_cast<int>(i)};
                }
            }
        }

        for (size_t i = 0; i < obstacleGroup.verticalLines.size(); ++i) {
            const auto& vLine = obstacleGroup.verticalLines[i];
            for (int j = 0; j < vLine.count; ++j) {
                float worldX = vLine.posX;
                float worldY = vLine.fromY + (j * spriteData.height);
                float screenX = levelX + (worldX * _viewportZoom);
                float screenY = levelY + (worldY * _viewportZoom);

                if (mouseX >= screenX && mouseX < screenX + scaledWidth &&
                    mouseY >= screenY && mouseY < screenY + scaledHeight) {
                    return ObstacleSelection{prefabName, "vertical", static_cast<int>(i)};
                }
            }
        }
    }

    return std::nullopt;
}

void LevelEditorState::handleObstacleClick(
    float mouseX, float mouseY, float levelX, float levelY
) {
    auto selection = getObstacleAtPosition(mouseX, mouseY, levelX, levelY);
    if (selection.has_value()) {
        _selectedObstacle = selection;
        if (_obstaclePrefabDropdown) {
            const auto& prefabName = selection.value().prefabName;
            auto options = _obstaclePrefabDropdown->getOptions();
            for (size_t i = 0; i < options.size(); ++i) {
                if (options[i] == prefabName) {
                    _obstaclePrefabDropdown->setSelectedIndex(i);
                    break;
                }
            }
        }
        if (_editorModeDropdown) {
            _editorModeDropdown->setSelectedIndex(0);
        }

        if (_obstacleTypeDropdown) {
            const auto& obstacleType = selection.value().type;
            size_t typeIndex = 0;
            if (obstacleType == "unique") {
                typeIndex = 0;
            } else if (obstacleType == "horizontal") {
                typeIndex = 1;
            } else if (obstacleType == "vertical") {
                typeIndex = 2;
            }
            _obstacleTypeDropdown->setSelectedIndex(typeIndex);
        }

        const auto& sel = selection.value();
        float posX = 0.0f;
        float posY = 0.0f;

        if (sel.type == "unique") {
            const auto& obstacle = _obstaclesByName[sel.prefabName].uniques[
                static_cast<size_t>(sel.index)];
            posX = obstacle.posX;
            posY = obstacle.posY;
        } else if (sel.type == "horizontal") {
            const auto& obstacle = _obstaclesByName[sel.prefabName].horizontalLines[
                static_cast<size_t>(sel.index)];
            posX = obstacle.fromX;
            posY = obstacle.posY;
        } else if (sel.type == "vertical") {
            const auto& obstacle = _obstaclesByName[sel.prefabName].verticalLines[
                static_cast<size_t>(sel.index)];
            posX = obstacle.posX;
            posY = obstacle.fromY;
        }

        if (_obstaclePosXInput) {
            _obstaclePosXInput->setText(std::to_string(static_cast<int>(posX)));
            _obstaclePosXInput->setVisible(true);
        }
        if (_obstaclePosXLabel) {
            _obstaclePosXLabel->setVisible(true);
        }
        if (_obstaclePosYInput) {
            _obstaclePosYInput->setText(std::to_string(static_cast<int>(posY)));
            _obstaclePosYInput->setVisible(true);
        }
        if (_obstaclePosYLabel) {
            _obstaclePosYLabel->setVisible(true);
        }

        if (sel.type == "horizontal") {
            const auto& obstacle = _obstaclesByName[sel.prefabName].horizontalLines[
                static_cast<size_t>(sel.index)];
            if (_obstacleCountInput) {
                _obstacleCountInput->setText(std::to_string(obstacle.count));
                _obstacleCountInput->setVisible(true);
            }
            if (_obstacleCountLabel) {
                _obstacleCountLabel->setVisible(true);
            }
        } else if (sel.type == "vertical") {
            const auto& obstacle = _obstaclesByName[sel.prefabName].verticalLines[
                static_cast<size_t>(sel.index)];
            if (_obstacleCountInput) {
                _obstacleCountInput->setText(std::to_string(obstacle.count));
                _obstacleCountInput->setVisible(true);
            }
            if (_obstacleCountLabel) {
                _obstacleCountLabel->setVisible(true);
            }
        } else {
            if (_obstacleCountInput) {
                _obstacleCountInput->setVisible(false);
            }
            if (_obstacleCountLabel) {
                _obstacleCountLabel->setVisible(false);
            }
        }

        if (_obstacleDeleteButton) {
            _obstacleDeleteButton->setVisible(true);
        }
    } else {
        bool inObstaclesMode = (_editorModeDropdown &&
            _editorModeDropdown->getSelectedOption() == "Obstacles");

        std::string selectedPrefab;
        if (_obstaclePrefabDropdown) {
            selectedPrefab = _obstaclePrefabDropdown->getSelectedOption();
        }

        if (inObstaclesMode && !selectedPrefab.empty() && !_selectedObstacle.has_value()) {
            float levelMouseX = (mouseX - levelX) / _viewportZoom;
            float levelMouseY = (mouseY - levelY) / _viewportZoom;

            std::string obstacleType = "unique";
            if (_obstacleTypeDropdown) {
                size_t typeIndex = _obstacleTypeDropdown->getSelectedIndex();
                if (typeIndex == 1) {
                    obstacleType = "horizontal";
                } else if (typeIndex == 2) {
                    obstacleType = "vertical";
                } else {
                    obstacleType = "unique";
                }
            }

            if (_obstaclesByName.find(selectedPrefab) == _obstaclesByName.end()) {
                _obstaclesByName[selectedPrefab] = ObstacleGroup();
            }

            int newIndex = 0;
            ObstacleSelection newSelection;
            newSelection.prefabName = selectedPrefab;
            newSelection.type = obstacleType;

            if (obstacleType == "horizontal") {
                HorizontalLineObstacle newObstacle;
                newObstacle.fromX = levelMouseX;
                newObstacle.posY = levelMouseY;
                newObstacle.count = 1;
                _obstaclesByName[selectedPrefab].horizontalLines.push_back(newObstacle);
                newIndex = static_cast<int>(_obstaclesByName[selectedPrefab].horizontalLines.size() - 1);
            } else if (obstacleType == "vertical") {
                VerticalLineObstacle newObstacle;
                newObstacle.posX = levelMouseX;
                newObstacle.fromY = levelMouseY;
                newObstacle.count = 1;
                _obstaclesByName[selectedPrefab].verticalLines.push_back(newObstacle);
                newIndex = static_cast<int>(_obstaclesByName[selectedPrefab].verticalLines.size() - 1);
            } else {
                UniqueObstacle newObstacle;
                newObstacle.posX = levelMouseX;
                newObstacle.posY = levelMouseY;
                _obstaclesByName[selectedPrefab].uniques.push_back(newObstacle);
                newIndex = static_cast<int>(_obstaclesByName[selectedPrefab].uniques.size() - 1);
            }

            newSelection.index = newIndex;
            _selectedObstacle = newSelection;

            if (_obstacleTypeDropdown) {
                if (obstacleType == "horizontal") {
                    _obstacleTypeDropdown->setSelectedIndex(1);
                } else if (obstacleType == "vertical") {
                    _obstacleTypeDropdown->setSelectedIndex(2);
                } else {
                    _obstacleTypeDropdown->setSelectedIndex(0);
                }
            }

            if (_obstaclePosXInput) {
                if (obstacleType == "vertical") {
                    _obstaclePosXInput->setText(std::to_string(static_cast<int>(levelMouseX)));
                } else {
                    _obstaclePosXInput->setText(std::to_string(static_cast<int>(levelMouseX)));
                }
                _obstaclePosXInput->setVisible(true);
            }
            if (_obstaclePosXLabel) {
                _obstaclePosXLabel->setVisible(true);
            }
            if (_obstaclePosYInput) {
                if (obstacleType == "vertical") {
                    _obstaclePosYInput->setText(std::to_string(static_cast<int>(levelMouseY)));
                } else {
                    _obstaclePosYInput->setText(std::to_string(static_cast<int>(levelMouseY)));
                }
                _obstaclePosYInput->setVisible(true);
            }
            if (_obstaclePosYLabel) {
                _obstaclePosYLabel->setVisible(true);
            }

            if (obstacleType == "horizontal" || obstacleType == "vertical") {
                if (_obstacleCountInput) {
                    _obstacleCountInput->setText("1");
                    _obstacleCountInput->setVisible(true);
                }
                if (_obstacleCountLabel) {
                    _obstacleCountLabel->setVisible(true);
                }
            } else {
                if (_obstacleCountInput) {
                    _obstacleCountInput->setVisible(false);
                }
                if (_obstacleCountLabel) {
                    _obstacleCountLabel->setVisible(false);
                }
            }

            if (_obstacleDeleteButton) {
                _obstacleDeleteButton->setVisible(true);
            }

            _hasUnsavedChanges = true;
            updateSaveButtonText();
        } else {
            _selectedObstacle = std::nullopt;
            if (_obstaclePosXInput) {
                _obstaclePosXInput->setVisible(false);
            }
            if (_obstaclePosXLabel) {
                _obstaclePosXLabel->setVisible(false);
            }
            if (_obstaclePosYInput) {
                _obstaclePosYInput->setVisible(false);
            }
            if (_obstaclePosYLabel) {
                _obstaclePosYLabel->setVisible(false);
            }
            if (_obstacleCountInput) {
                _obstacleCountInput->setVisible(false);
            }
            if (_obstacleCountLabel) {
                _obstacleCountLabel->setVisible(false);
            }
            if (_obstacleDeleteButton) {
                _obstacleDeleteButton->setVisible(false);
            }
        }
    }
}

void LevelEditorState::startObstacleDrag(
    math::Vector2f mousePos,
    float viewportZoom,
    float sidePanelWidth
) {
    if (!_selectedObstacle.has_value()) {
        _isDraggingObstacle = false;
        return;
    }

    const auto& sel = _selectedObstacle.value();
    float obstacleWorldX = 0.0f;
    float obstacleWorldY = 0.0f;

    if (sel.type == "unique") {
        obstacleWorldX = _obstaclesByName[sel.prefabName].uniques[
            static_cast<size_t>(sel.index)].posX;
        obstacleWorldY = _obstaclesByName[sel.prefabName].uniques[
            static_cast<size_t>(sel.index)].posY;
    } else if (sel.type == "horizontal") {
        obstacleWorldX = _obstaclesByName[sel.prefabName].horizontalLines[
            static_cast<size_t>(sel.index)].fromX;
        obstacleWorldY = _obstaclesByName[sel.prefabName].horizontalLines[
            static_cast<size_t>(sel.index)].posY;
    } else if (sel.type == "vertical") {
        obstacleWorldX = _obstaclesByName[sel.prefabName].verticalLines[
            static_cast<size_t>(sel.index)].posX;
        obstacleWorldY = _obstaclesByName[sel.prefabName].verticalLines[
            static_cast<size_t>(sel.index)].fromY;
    }

    float obstacleScreenX =
        sidePanelWidth + (obstacleWorldX - _viewportOffset.getX()) * viewportZoom;
    float obstacleScreenY = (obstacleWorldY - _viewportOffset.getY()) * viewportZoom;

    _dragObstacleOffset = math::Vector2f(
        obstacleScreenX - mousePos.getX(),
        obstacleScreenY - mousePos.getY()
    );
}

void LevelEditorState::handleObstacleDrag(
    math::Vector2f mousePos,
    float viewportZoom,
    float sidePanelWidth
) {
    if (!_selectedObstacle.has_value()) {
        _isDraggingObstacle = false;
        return;
    }

    const auto& sel = _selectedObstacle.value();

    float newScreenX = mousePos.getX() + _dragObstacleOffset.getX();
    float newScreenY = mousePos.getY() + _dragObstacleOffset.getY();

    float newWorldX = _viewportOffset.getX() + (newScreenX - sidePanelWidth) / viewportZoom;
    float newWorldY = _viewportOffset.getY() + newScreenY / viewportZoom;

    if (sel.type == "unique") {
        _obstaclesByName[sel.prefabName].uniques[
            static_cast<size_t>(sel.index)].posX = newWorldX;
        _obstaclesByName[sel.prefabName].uniques[
            static_cast<size_t>(sel.index)].posY = newWorldY;
    } else if (sel.type == "horizontal") {
        _obstaclesByName[sel.prefabName].horizontalLines[
            static_cast<size_t>(sel.index)].fromX = newWorldX;
        _obstaclesByName[sel.prefabName].horizontalLines[
            static_cast<size_t>(sel.index)].posY = newWorldY;
    } else if (sel.type == "vertical") {
        _obstaclesByName[sel.prefabName].verticalLines[
            static_cast<size_t>(sel.index)].posX = newWorldX;
        _obstaclesByName[sel.prefabName].verticalLines[
            static_cast<size_t>(sel.index)].fromY = newWorldY;
    }

    float posX = 0.0f;
    float posY = 0.0f;
    if (sel.type == "unique") {
        const auto& obstacle = _obstaclesByName[sel.prefabName].uniques[
            static_cast<size_t>(sel.index)];
        posX = obstacle.posX;
        posY = obstacle.posY;
    } else if (sel.type == "horizontal") {
        const auto& obstacle = _obstaclesByName[sel.prefabName].horizontalLines[
            static_cast<size_t>(sel.index)];
        posX = obstacle.fromX;
        posY = obstacle.posY;
    } else if (sel.type == "vertical") {
        const auto& obstacle = _obstaclesByName[sel.prefabName].verticalLines[
            static_cast<size_t>(sel.index)];
        posX = obstacle.posX;
        posY = obstacle.fromY;
    }

    if (_obstaclePosXInput) {
        _obstaclePosXInput->setText(std::to_string(static_cast<int>(posX)));
    }
    if (_obstaclePosYInput) {
        _obstaclePosYInput->setText(std::to_string(static_cast<int>(posY)));
    }

    _hasUnsavedChanges = true;
    updateSaveButtonText();
}

}  // namespace gsm
