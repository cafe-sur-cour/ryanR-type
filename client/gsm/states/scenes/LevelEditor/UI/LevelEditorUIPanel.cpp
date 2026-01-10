/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUIPanel
*/

#include <filesystem>  // NOLINT(build/c++17)
#include <memory>
#include <string>
#include <algorithm>
#include <utility>
#include <fstream>
#include "../LevelEditorState.hpp"
#include "../../../../../constants.hpp"
#include "../../../../../colors.hpp"

namespace gsm {

void LevelEditorState::createBottomPanel() {
    createBottomPanelBase();
    createEditorModeDropdown();
    createObstacleUI();
    createPowerUpUI();
    createWaveUI();
    createEnemyUI();
}

void LevelEditorState::createBottomPanelBase() {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;
    const float canvasWidth = constants::MAX_WIDTH - sidePanelWidth;

    _bottomPanel = std::make_shared<ui::Panel>(_resourceManager);
    _bottomPanel->setPosition(math::Vector2f(sidePanelWidth, canvasHeight));
    _bottomPanel->setSize(math::Vector2f(canvasWidth, bottomPanelHeight));
    _bottomPanel->setBackgroundColor(colors::LIGHT_GRAY);
    _bottomPanel->setBorderColor(colors::GRAY);
    _uiManager->addElement(_bottomPanel);
}

void LevelEditorState::createEditorModeDropdown() {
    _editorModeDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _editorModeDropdown->setPosition(math::Vector2f(10.0f, 35.0f));
    _editorModeDropdown->setSize(math::Vector2f(200.0f, 40.0f));
    _editorModeDropdown->setOptions({"Obstacles", "PowerUps", "Waves"});
    _editorModeDropdown->setDirection(ui::DropdownDirection::Right);
    _editorModeDropdown->setSelectedIndex(0);
    _editorModeDropdown->setOnSelectionChanged(
        [this](const std::string& mode, [[maybe_unused]] size_t index) {
        bool showObstacles = (mode == "Obstacles");
        bool showPowerUps = (mode == "PowerUps");
        bool showWaves = (mode == "Waves");

        if (showObstacles && _selectedPowerUp.has_value()) {
            _selectedPowerUp = std::nullopt;
            hidePowerUpUI();
        }
        if ((showPowerUps || showWaves) && _selectedObstacle.has_value()) {
            _selectedObstacle = std::nullopt;
            hideObstacleUI();
        }

        if (_spritePreview) {
            _spritePreview->clear();
        }
        if (_spriteWidthLabel) {
            _spriteWidthLabel->setText("Width: 0");
        }
        if (_spriteHeightLabel) {
            _spriteHeightLabel->setText("Height: 0");
        }

        if (showObstacles && _obstaclePrefabDropdown) {
            std::string selectedPrefab = _obstaclePrefabDropdown->getSelectedOption();
            if (!selectedPrefab.empty()) {
                std::filesystem::path prefabPath =
                    std::filesystem::path(constants::OBSTACLES_DIRECTORY) / (selectedPrefab +
                        constants::LEVEL_FILE_EXTENSION);
                if (_spritePreview && std::filesystem::exists(prefabPath)) {
                    _spritePreview->loadPrefab(prefabPath);
                    LevelPreviewSprite spriteData =
                        extractSpriteDataFromPrefab(prefabPath.string());
                    _spriteWidthLabel->setText(
                        "Width: " + std::to_string(static_cast<int>(spriteData.width)));
                    _spriteHeightLabel->setText(
                        "Height: " + std::to_string(static_cast<int>(spriteData.height)));
                }
            }
        } else if (showPowerUps && _powerUpPrefabDropdown) {
            std::string selectedPrefab = _powerUpPrefabDropdown->getSelectedOption();
            if (!selectedPrefab.empty()) {
                std::filesystem::path prefabPath =
                    std::filesystem::path(constants::POWERUPS_DIRECTORY) / (selectedPrefab +
                        constants::LEVEL_FILE_EXTENSION);
                if (_spritePreview && std::filesystem::exists(prefabPath)) {
                    _spritePreview->loadPrefab(prefabPath);
                    LevelPreviewSprite spriteData =
                        extractSpriteDataFromPrefab(prefabPath.string());
                    _spriteWidthLabel->setText(
                        "Width: " + std::to_string(static_cast<int>(spriteData.width)));
                    _spriteHeightLabel->setText(
                        "Height: " + std::to_string(static_cast<int>(spriteData.height)));
                }
            }
        }

        if (_obstaclePrefabLabel) {
            _obstaclePrefabLabel->setVisible(showObstacles);
        }
        if (_obstaclePrefabDropdown) {
            _obstaclePrefabDropdown->setVisible(showObstacles);
        }
        if (_powerUpPrefabLabel) {
            _powerUpPrefabLabel->setVisible(showPowerUps);
        }
        if (_powerUpPrefabDropdown) {
            _powerUpPrefabDropdown->setVisible(showPowerUps);
        }
        if (_spritePreviewPanel) {
            _spritePreviewPanel->setVisible(showObstacles || showPowerUps);
        }
        if (_spritePreview) {
            _spritePreview->setVisible(showObstacles || showPowerUps);
        }
        if (_spriteWidthLabel) {
            _spriteWidthLabel->setVisible(showObstacles || showPowerUps);
        }
        if (_spriteHeightLabel) {
            _spriteHeightLabel->setVisible(showObstacles || showPowerUps);
        }
        if (_obstacleTypeLabel) {
            _obstacleTypeLabel->setVisible(showObstacles);
        }
        if (_obstacleTypeDropdown) {
            _obstacleTypeDropdown->setVisible(showObstacles);
        }

        if (_waveLabel) {
            _waveLabel->setVisible(showWaves);
        }
        if (_waveIndexLabel) {
            _waveIndexLabel->setVisible(showWaves);
            if (showWaves) {
                if (_selectedWave.has_value() && !_waves.empty()) {
                    _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                        " / " + std::to_string(_waves.size()));
                } else {
                    _waveIndexLabel->setText("0 / " + std::to_string(_waves.size()));
                }
            }
        }
        if (_wavePrevButton) {
            _wavePrevButton->setVisible(showWaves && !_waves.empty());
        }
        if (_waveNextButton) {
            _waveNextButton->setVisible(showWaves && !_waves.empty());
        }
        if (_waveTriggerXLabel) {
            _waveTriggerXLabel->setVisible(showWaves && _selectedWave.has_value());
        }
        if (_waveTriggerXInput) {
            _waveTriggerXInput->setVisible(showWaves && _selectedWave.has_value());
        }
        if (_waveDeleteButton) {
            _waveDeleteButton->setVisible(showWaves && _selectedWave.has_value());
        }

        if (!showWaves) {
            _selectedWave = std::nullopt;
            hideWaveUI();
            hideEnemyUI();
        }
    });
    _editorModeDropdown->setScalingEnabled(false);
    _editorModeDropdown->setFocusEnabled(true);
    _bottomPanel->addChild(_editorModeDropdown);
}

}  // namespace gsm
