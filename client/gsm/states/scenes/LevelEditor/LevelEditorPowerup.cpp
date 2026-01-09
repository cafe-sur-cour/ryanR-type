/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorPowerup
*/

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include "LevelEditorState.hpp"
#include "../../../../constants.hpp"

namespace gsm {

void LevelEditorState::parsePowerUps() {
    _powerUpsByName.clear();
    _powerUpAnimationData.clear();
    _powerUpAnimationFrames.clear();
    _powerUpAnimationTimes.clear();

    if (!_levelData.contains(constants::LEVEL_POWER_UPS_FIELD) ||
        !_levelData[constants::LEVEL_POWER_UPS_FIELD].is_array()) {
        return;
    }

    const auto& powerUps = _levelData[constants::LEVEL_POWER_UPS_FIELD];

    for (const auto& powerUp : powerUps) {
        if (!powerUp.contains(constants::POWERUP_NAME_FIELD)) {
            continue;
        }
        std::string powerUpName =
            powerUp[constants::POWERUP_NAME_FIELD].get<std::string>();

        if (_powerUpsByName.find(powerUpName) == _powerUpsByName.end()) {
            _powerUpsByName[powerUpName] = std::vector<PowerUpData>();
        }

        if (!powerUp.contains(constants::POWERUP_POSITION_FIELD)) {
            continue;
        }

        const auto& position = powerUp[constants::POWERUP_POSITION_FIELD];

        if (!position.contains(constants::OBSTACLE_POSX_FIELD) ||
            !position.contains(constants::OBSTACLE_POSY_FIELD)) {
            continue;
        }

        PowerUpData data;
        data.posX = position[constants::OBSTACLE_POSX_FIELD].get<float>();
        data.posY = position[constants::OBSTACLE_POSY_FIELD].get<float>();

        _powerUpsByName[powerUpName].push_back(data);

        std::filesystem::path prefabPath =
            std::filesystem::path(constants::POWERUPS_DIRECTORY) / (powerUpName +
                constants::LEVEL_FILE_EXTENSION);
        if (std::filesystem::exists(prefabPath)) {
            LevelPreviewSprite spriteData = extractSpriteDataFromPrefab(prefabPath.string());
            _powerUpAnimationData[powerUpName] = spriteData;
            if (spriteData.isAnimation) {
                _powerUpAnimationFrames[powerUpName] = 0.0f;
                _powerUpAnimationTimes[powerUpName] = 0.0f;
            }
        }
    }
}

void LevelEditorState::renderAllPowerUps(
    float levelX,
    float levelY,
    float canvasLeft,
    float canvasRight,
    float canvasTop,
    float canvasBottom
) {
    auto window = _resourceManager->get<gfx::IWindow>();
    gfx::color_t lightGreen = {100, 255, 100, 255};

    for (const auto& [prefabName, powerUpVec] : _powerUpsByName) {
        LevelPreviewSprite spriteData;
        if (_powerUpAnimationData.find(prefabName) != _powerUpAnimationData.end()) {
            spriteData = _powerUpAnimationData[prefabName];
        } else {
            std::string prefabPath =
                constants::POWERUPS_DIRECTORY + "/" +
                    prefabName + constants::LEVEL_FILE_EXTENSION;
            spriteData = extractSpriteDataFromPrefab(prefabPath);
            _powerUpAnimationData[prefabName] = spriteData;
        }

        if (spriteData.texturePath.empty()) {
            continue;
        }

        if (spriteData.isAnimation && _powerUpAnimationFrames.find(prefabName)
            == _powerUpAnimationFrames.end()) {
            _powerUpAnimationFrames[prefabName] = 0.0f;
            _powerUpAnimationTimes[prefabName] = 0.0f;
        }

        float scaledWidth = spriteData.width * _viewportZoom;
        float scaledHeight = spriteData.height * _viewportZoom;

        for (size_t i = 0; i < powerUpVec.size(); ++i) {
            const auto& powerUp = powerUpVec[i];
            float screenX = levelX + (powerUp.posX * _viewportZoom);
            float screenY = levelY + (powerUp.posY * _viewportZoom);

            renderSpriteInLevelPreview(
                spriteData, prefabName, screenX, screenY, canvasLeft,
                canvasRight, canvasTop, canvasBottom);

            if (_selectedPowerUp.has_value() &&
                _selectedPowerUp.value().prefabName == prefabName &&
                _selectedPowerUp.value().index == static_cast<int>(i)) {
                window->drawRectangleOutline(
                    lightGreen,
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

void LevelEditorState::savePowerUps() {
    nlohmann::json powerUpsArray = nlohmann::json::array();

    for (const auto& [prefabName, powerUpVec] : _powerUpsByName) {
        for (const auto& powerUp : powerUpVec) {
            nlohmann::json powerUpEntry;
            powerUpEntry[constants::POWERUP_NAME_FIELD] = prefabName;

            nlohmann::json position;
            position[constants::OBSTACLE_POSX_FIELD] = powerUp.posX;
            position[constants::OBSTACLE_POSY_FIELD] = powerUp.posY;

            powerUpEntry[constants::POWERUP_POSITION_FIELD] = position;
            powerUpsArray.push_back(powerUpEntry);
        }
    }

    _levelData[constants::LEVEL_POWER_UPS_FIELD] = powerUpsArray;
}

std::optional<PowerUpSelection> LevelEditorState::getPowerUpAtPosition(
    float mouseX, float mouseY, float levelX, float levelY
) {
    for (const auto& [prefabName, powerUpVec] : _powerUpsByName) {
        std::string prefabPath =
            constants::POWERUPS_DIRECTORY + "/" +
                prefabName + constants::LEVEL_FILE_EXTENSION;
        LevelPreviewSprite spriteData = extractSpriteDataFromPrefab(prefabPath);

        if (spriteData.texturePath.empty()) {
            continue;
        }

        float scaledWidth = spriteData.width * _viewportZoom;
        float scaledHeight = spriteData.height * _viewportZoom;

        for (size_t i = 0; i < powerUpVec.size(); ++i) {
            const auto& powerUp = powerUpVec[i];
            float screenX = levelX + (powerUp.posX * _viewportZoom);
            float screenY = levelY + (powerUp.posY * _viewportZoom);

            if (mouseX >= screenX && mouseX < screenX + scaledWidth &&
                mouseY >= screenY && mouseY < screenY + scaledHeight) {
                return PowerUpSelection{prefabName, static_cast<int>(i)};
            }
        }
    }

    return std::nullopt;
}

void LevelEditorState::handlePowerUpClick(
    float mouseX, float mouseY, float levelX, float levelY
) {
    auto selection = getPowerUpAtPosition(mouseX, mouseY, levelX, levelY);
    if (selection.has_value()) {
        _isSelectingObject = true;
        _selectedPowerUp = selection;
        _selectedObstacle = std::nullopt;
        _selectedWave = std::nullopt;

        hideObstacleUI();
        hideWaveUI();
        hideEnemyUI();

        if (_powerUpPrefabDropdown) {
            const auto& prefabName = selection.value().prefabName;
            auto options = _powerUpPrefabDropdown->getOptions();
            for (size_t i = 0; i < options.size(); ++i) {
                if (options[i] == prefabName) {
                    _powerUpPrefabDropdown->setSelectedIndex(i);
                    break;
                }
            }
        }
        if (_editorModeDropdown) {
            _editorModeDropdown->setSelectedIndex(1);
        }

        const auto& sel = selection.value();
        const auto& powerUp = _powerUpsByName[sel.prefabName][
            static_cast<size_t>(sel.index)];
        float posX = powerUp.posX;
        float posY = powerUp.posY;

        if (_powerUpPosXInput) {
            _powerUpPosXInput->setText(std::to_string(static_cast<int>(posX)));
        }
        if (_powerUpPosYInput) {
            _powerUpPosYInput->setText(std::to_string(static_cast<int>(posY)));
        }

        showPowerUpUI();
        _isSelectingObject = false;
    } else {
        bool inPowerUpsMode = (_editorModeDropdown &&
            _editorModeDropdown->getSelectedOption() == "PowerUps");

        std::string selectedPrefab;
        if (_powerUpPrefabDropdown) {
            selectedPrefab = _powerUpPrefabDropdown->getSelectedOption();
        }

        bool filterAllowsCreation = (_displayFilter == "All" || _displayFilter == "PowerUps");

        if (inPowerUpsMode && !selectedPrefab.empty() && !_selectedPowerUp.has_value() &&
            filterAllowsCreation) {
            float levelMouseX = (mouseX - levelX) / _viewportZoom;
            float levelMouseY = (mouseY - levelY) / _viewportZoom;

            if (_powerUpsByName.find(selectedPrefab) == _powerUpsByName.end()) {
                _powerUpsByName[selectedPrefab] = std::vector<PowerUpData>();
            }

            PowerUpData newPowerUp;
            newPowerUp.posX = levelMouseX;
            newPowerUp.posY = levelMouseY;
            _powerUpsByName[selectedPrefab].push_back(newPowerUp);

            int newIndex = static_cast<int>(
                _powerUpsByName[selectedPrefab].size() - 1);

            PowerUpSelection newSelection;
            newSelection.prefabName = selectedPrefab;
            newSelection.index = newIndex;
            _selectedPowerUp = newSelection;

            if (_powerUpPosXInput) {
                _powerUpPosXInput->setText(std::to_string(static_cast<int>(levelMouseX)));
                _powerUpPosXInput->setVisible(true);
            }
            if (_powerUpPosXLabel) {
                _powerUpPosXLabel->setVisible(true);
            }
            if (_powerUpPosYInput) {
                _powerUpPosYInput->setText(std::to_string(static_cast<int>(levelMouseY)));
                _powerUpPosYInput->setVisible(true);
            }
            if (_powerUpPosYLabel) {
                _powerUpPosYLabel->setVisible(true);
            }
            if (_powerUpDeleteButton) {
                _powerUpDeleteButton->setVisible(true);
            }
            if (_powerUpDuplicateButton) {
                _powerUpDuplicateButton->setVisible(true);
            }

            _hasUnsavedChanges = true;
            updateSaveButtonText();
            saveToHistory();
        } else {
            _selectedPowerUp = std::nullopt;
            hidePowerUpUI();
        }
    }
}

void LevelEditorState::startPowerUpDrag(
    math::Vector2f mousePos,
    float viewportZoom,
    float sidePanelWidth
) {
    if (!_selectedPowerUp.has_value()) {
        _isDraggingPowerUp = false;
        return;
    }

    const auto& sel = _selectedPowerUp.value();
    float powerUpWorldX = _powerUpsByName[sel.prefabName][
        static_cast<size_t>(sel.index)].posX;
    float powerUpWorldY = _powerUpsByName[sel.prefabName][
        static_cast<size_t>(sel.index)].posY;

    float powerUpScreenX =
        sidePanelWidth + (powerUpWorldX - _viewportOffset.getX()) * viewportZoom;
    float powerUpScreenY = (powerUpWorldY - _viewportOffset.getY()) * viewportZoom;

    _dragPowerUpOffset = math::Vector2f(
        powerUpScreenX - mousePos.getX(),
        powerUpScreenY - mousePos.getY()
    );
}

void LevelEditorState::handlePowerUpDrag(
    math::Vector2f mousePos,
    float viewportZoom,
    float sidePanelWidth
) {
    if (!_selectedPowerUp.has_value()) {
        _isDraggingPowerUp = false;
        return;
    }

    const auto& sel = _selectedPowerUp.value();

    float newScreenX = mousePos.getX() + _dragPowerUpOffset.getX();
    float newScreenY = mousePos.getY() + _dragPowerUpOffset.getY();

    float newWorldX = _viewportOffset.getX() + (newScreenX - sidePanelWidth) / viewportZoom;
    float newWorldY = _viewportOffset.getY() + newScreenY / viewportZoom;

    _powerUpsByName[sel.prefabName][
        static_cast<size_t>(sel.index)].posX = newWorldX;
    _powerUpsByName[sel.prefabName][
        static_cast<size_t>(sel.index)].posY = newWorldY;

    const auto& powerUp = _powerUpsByName[sel.prefabName][
        static_cast<size_t>(sel.index)];
    float posX = powerUp.posX;
    float posY = powerUp.posY;

    if (_powerUpPosXInput) {
        _powerUpPosXInput->setText(std::to_string(static_cast<int>(posX)));
    }
    if (_powerUpPosYInput) {
        _powerUpPosYInput->setText(std::to_string(static_cast<int>(posY)));
    }

    _hasUnsavedChanges = true;
    updateSaveButtonText();
}

}  // namespace gsm
