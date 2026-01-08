/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorWaves
*/

#include <cmath>
#include <fstream>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>
#include "LevelEditorState.hpp"
#include "../../../../constants.hpp"

namespace gsm {

void LevelEditorState::parseWaves() {
    _waves.clear();

    if (!_levelData.contains(constants::LEVEL_WAVES_FIELD) ||
        !_levelData[constants::LEVEL_WAVES_FIELD].is_array()) {
        return;
    }

    const auto& wavesArray = _levelData[constants::LEVEL_WAVES_FIELD];

    for (const auto& waveJson : wavesArray) {
        Wave wave;

        if (!waveJson.contains(constants::WAVE_GAMEX_TRIGGER_FIELD)) {
            continue;
        }
        wave.gameXTrigger = waveJson[constants::WAVE_GAMEX_TRIGGER_FIELD].get<float>();

        if (waveJson.contains(constants::WAVE_ENEMIES_FIELD) &&
            waveJson[constants::WAVE_ENEMIES_FIELD].is_array()) {
            const auto& enemiesArray = waveJson[constants::WAVE_ENEMIES_FIELD];

            for (const auto& enemyJson : enemiesArray) {
                WaveEnemy enemy;

                if (!enemyJson.contains(constants::WAVE_ENEMY_TYPE_FIELD)) {
                    continue;
                }
                enemy.type = enemyJson[constants::WAVE_ENEMY_TYPE_FIELD].get<std::string>();
                enemy.count = enemyJson.value(constants::WAVE_ENEMY_COUNT_FIELD, 1);

                if (enemyJson.contains(constants::WAVE_DISTRIBUTION_X_FIELD)) {
                    const auto& distX = enemyJson[constants::WAVE_DISTRIBUTION_X_FIELD];
                    enemy.distributionX.min =
                        distX.value(constants::WAVE_DISTRIBUTION_MIN_FIELD, 0.0f);
                    enemy.distributionX.max =
                        distX.value(constants::WAVE_DISTRIBUTION_MAX_FIELD, 100.0f);
                    enemy.distributionX.type =
                        distX.value(constants::WAVE_DISTRIBUTION_TYPE_FIELD, "uniform");
                }

                if (enemyJson.contains(constants::WAVE_DISTRIBUTION_Y_FIELD)) {
                    const auto& distY = enemyJson[constants::WAVE_DISTRIBUTION_Y_FIELD];
                    enemy.distributionY.min =
                        distY.value(constants::WAVE_DISTRIBUTION_MIN_FIELD, 0.0f);
                    enemy.distributionY.max =
                        distY.value(constants::WAVE_DISTRIBUTION_MAX_FIELD, 100.0f);
                    enemy.distributionY.type =
                        distY.value(constants::WAVE_DISTRIBUTION_TYPE_FIELD, "uniform");
                }

                wave.enemies.push_back(enemy);
            }
        }

        _waves.push_back(wave);
    }
}

void LevelEditorState::renderAllWaves(
    float levelX,
    float levelY,
    float canvasLeft,
    float canvasRight,
    float canvasTop,
    float canvasBottom
) {
    auto window = _resourceManager->get<gfx::IWindow>();

    float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);
    if (mapLength <= 0.0f) {
        return;
    }

    for (size_t waveIdx = 0; waveIdx < _waves.size(); ++waveIdx) {
        const auto& wave = _waves[waveIdx];

        float hue = (waveIdx * 360.0f / 7.0f);
        while (hue >= 360.0f) hue -= 360.0f;

        float h = hue / 60.0f;
        float c = 0.8f;
        float x = c * (1.0f - std::abs(std::fmod(h, 2.0f) - 1.0f));
        float r = 0.0f, g = 0.0f, b = 0.0f;

        if (h >= 0 && h < 1) {
            r = c; g = x; b = 0;
        } else if (h >= 1 && h < 2) {
            r = x; g = c; b = 0;
        } else if (h >= 2 && h < 3) {
            r = 0; g = c; b = x;
        } else if (h >= 3 && h < 4) {
            r = 0; g = x; b = c;
        } else if (h >= 4 && h < 5) {
            r = x; g = 0; b = c;
        } else {
            r = c; g = 0; b = x;
        }

        float m = 0.2f;
        r += m; g += m; b += m;

        gfx::color_t waveColor = {
            static_cast<uint8_t>(r * 255),
            static_cast<uint8_t>(g * 255),
            static_cast<uint8_t>(b * 255),
            128
        };

        gfx::color_t waveColorLight = {
            static_cast<uint8_t>(r * 255),
            static_cast<uint8_t>(g * 255),
            static_cast<uint8_t>(b * 255),
            80
        };

        float triggerScreenX = levelX + (wave.gameXTrigger * _viewportZoom);

        if (triggerScreenX >= canvasLeft && triggerScreenX <= canvasRight) {
            float lineWidth = 3.0f;

            window->drawFilledRectangle(
                waveColor,
                std::make_pair(static_cast<size_t>(triggerScreenX),
                    static_cast<size_t>(canvasTop)),
                std::make_pair(static_cast<size_t>(lineWidth),
                    static_cast<size_t>(canvasBottom - canvasTop))
            );
        }

        for (size_t enemyIdx = 0; enemyIdx < wave.enemies.size(); ++enemyIdx) {
            const auto& enemy = wave.enemies[enemyIdx];

            float xMinPercent = enemy.distributionX.min;
            float xMaxPercent = enemy.distributionX.max;
            float yMinPercent = enemy.distributionY.min;
            float yMaxPercent = enemy.distributionY.max;

            float xMinScreen = (constants::MAX_WIDTH * xMinPercent) / 100.0f;
            float xMaxScreen = (constants::MAX_WIDTH * xMaxPercent) / 100.0f;
            float yMinScreen = (constants::MAX_HEIGHT * yMinPercent) / 100.0f;
            float yMaxScreen = (constants::MAX_HEIGHT * yMaxPercent) / 100.0f;

            float zoneCenterX = wave.gameXTrigger;

            float screenXMin = levelX + ((zoneCenterX + xMinScreen) * _viewportZoom);
            float screenXMax = levelX + ((zoneCenterX + xMaxScreen) * _viewportZoom);
            float screenYMin = levelY + (yMinScreen * _viewportZoom);
            float screenYMax = levelY + (yMaxScreen * _viewportZoom);

            if (screenXMax >= canvasLeft && screenXMin <= canvasRight &&
                screenYMax >= canvasTop && screenYMin <= canvasBottom) {
                float clippedXMin = (std::max)(screenXMin, canvasLeft);
                float clippedXMax = (std::min)(screenXMax, canvasRight);
                float clippedYMin = (std::max)(screenYMin, canvasTop);
                float clippedYMax = (std::min)(screenYMax, canvasBottom);

                float clippedWidth = clippedXMax - clippedXMin;
                float clippedHeight = clippedYMax - clippedYMin;

                if (clippedWidth > 0 && clippedHeight > 0) {
                    window->drawFilledRectangle(
                        waveColorLight,
                        std::make_pair(static_cast<size_t>(clippedXMin),
                            static_cast<size_t>(clippedYMin)),
                        std::make_pair(static_cast<size_t>(clippedWidth),
                            static_cast<size_t>(clippedHeight))
                    );

                    if (_selectedWave.has_value() &&
                        _selectedWave.value().waveIndex == static_cast<int>(waveIdx)) {
                        bool isEnemySelected =
                            _selectedWave.value().enemyIndex == static_cast<int>(enemyIdx);
                        bool isWaveSelected = _selectedWave.value().enemyIndex == -1;

                        if (isEnemySelected || isWaveSelected) {
                            gfx::color_t lightBlue = {100, 200, 255, 255};
                            window->drawRectangleOutline(
                                lightBlue,
                                std::make_pair(static_cast<size_t>(clippedXMin),
                                    static_cast<size_t>(clippedYMin)),
                                std::make_pair(static_cast<size_t>(clippedWidth),
                                    static_cast<size_t>(clippedHeight)),
                                isEnemySelected ? 3 : 2
                            );
                        }
                    }
                }
            }
        }

        if (_selectedWave.has_value() &&
            _selectedWave.value().waveIndex == static_cast<int>(waveIdx) &&
            _selectedWave.value().enemyIndex == -1 &&
            triggerScreenX >= canvasLeft && triggerScreenX <= canvasRight) {
            gfx::color_t lightBlue = {100, 200, 255, 255};
            float lineWidth = 5.0f;

            window->drawFilledRectangle(
                lightBlue,
                std::make_pair(static_cast<size_t>(triggerScreenX),
                    static_cast<size_t>(canvasTop)),
                std::make_pair(static_cast<size_t>(lineWidth),
                    static_cast<size_t>(canvasBottom - canvasTop))
            );
        }
    }
}

void LevelEditorState::saveWaves() {
    nlohmann::json wavesArray = nlohmann::json::array();

    for (const auto& wave : _waves) {
        nlohmann::json waveJson;
        waveJson[constants::WAVE_GAMEX_TRIGGER_FIELD] = wave.gameXTrigger;

        nlohmann::json enemiesArray = nlohmann::json::array();
        for (const auto& enemy : wave.enemies) {
            nlohmann::json enemyJson;
            enemyJson[constants::WAVE_ENEMY_TYPE_FIELD] = enemy.type;
            enemyJson[constants::WAVE_ENEMY_COUNT_FIELD] = enemy.count;

            nlohmann::json distX;
            distX[constants::WAVE_DISTRIBUTION_MIN_FIELD] = enemy.distributionX.min;
            distX[constants::WAVE_DISTRIBUTION_MAX_FIELD] = enemy.distributionX.max;
            distX[constants::WAVE_DISTRIBUTION_TYPE_FIELD] = enemy.distributionX.type;
            enemyJson[constants::WAVE_DISTRIBUTION_X_FIELD] = distX;

            nlohmann::json distY;
            distY[constants::WAVE_DISTRIBUTION_MIN_FIELD] = enemy.distributionY.min;
            distY[constants::WAVE_DISTRIBUTION_MAX_FIELD] = enemy.distributionY.max;
            distY[constants::WAVE_DISTRIBUTION_TYPE_FIELD] = enemy.distributionY.type;
            enemyJson[constants::WAVE_DISTRIBUTION_Y_FIELD] = distY;

            enemiesArray.push_back(enemyJson);
        }
        waveJson[constants::WAVE_ENEMIES_FIELD] = enemiesArray;

        wavesArray.push_back(waveJson);
    }

    _levelData[constants::LEVEL_WAVES_FIELD] = wavesArray;
}

std::optional<WaveSelection> LevelEditorState::getWaveAtPosition(
    float mouseX, float mouseY, float levelX, float levelY
) {
    float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);
    if (mapLength <= 0.0f) {
        return std::nullopt;
    }

    for (size_t waveIdx = 0; waveIdx < _waves.size(); ++waveIdx) {
        const auto& wave = _waves[waveIdx];

        for (size_t enemyIdx = 0; enemyIdx < wave.enemies.size(); ++enemyIdx) {
            const auto& enemy = wave.enemies[enemyIdx];

            float xMinPercent = enemy.distributionX.min;
            float xMaxPercent = enemy.distributionX.max;
            float yMinPercent = enemy.distributionY.min;
            float yMaxPercent = enemy.distributionY.max;

            float xMinScreen = (constants::MAX_WIDTH * xMinPercent) / 100.0f;
            float xMaxScreen = (constants::MAX_WIDTH * xMaxPercent) / 100.0f;
            float yMinScreen = (constants::MAX_HEIGHT * yMinPercent) / 100.0f;
            float yMaxScreen = (constants::MAX_HEIGHT * yMaxPercent) / 100.0f;

            float zoneCenterX = wave.gameXTrigger;
            float screenXMin = levelX + ((zoneCenterX + xMinScreen) * _viewportZoom);
            float screenXMax = levelX + ((zoneCenterX + xMaxScreen) * _viewportZoom);
            float screenYMin = levelY + (yMinScreen * _viewportZoom);
            float screenYMax = levelY + (yMaxScreen * _viewportZoom);

            if (mouseX >= screenXMin && mouseX <= screenXMax &&
                mouseY >= screenYMin && mouseY <= screenYMax) {
                return WaveSelection{static_cast<int>(waveIdx), static_cast<int>(enemyIdx)};
            }
        }
    }

    for (size_t waveIdx = 0; waveIdx < _waves.size(); ++waveIdx) {
        const auto& wave = _waves[waveIdx];
        float triggerScreenX = levelX + (wave.gameXTrigger * _viewportZoom);

        float clickTolerance = 10.0f;
        if (mouseX >= triggerScreenX - clickTolerance &&
            mouseX <= triggerScreenX + clickTolerance) {
            return WaveSelection{static_cast<int>(waveIdx), -1};
        }
    }

    return std::nullopt;
}

void LevelEditorState::handleWaveClick(
    float mouseX, float mouseY, float levelX, float levelY
) {
    auto selection = getWaveAtPosition(mouseX, mouseY, levelX, levelY);

    if (selection.has_value()) {
        _selectedWave = selection;
        _selectedObstacle = std::nullopt;
        _selectedPowerUp = std::nullopt;

        if (_obstaclePosXInput) _obstaclePosXInput->setVisible(false);
        if (_obstaclePosXLabel) _obstaclePosXLabel->setVisible(false);
        if (_obstaclePosYInput) _obstaclePosYInput->setVisible(false);
        if (_obstaclePosYLabel) _obstaclePosYLabel->setVisible(false);
        if (_obstacleCountInput) _obstacleCountInput->setVisible(false);
        if (_obstacleCountLabel) _obstacleCountLabel->setVisible(false);
        if (_obstacleDeleteButton) _obstacleDeleteButton->setVisible(false);

        if (_powerUpPosXInput) _powerUpPosXInput->setVisible(false);
        if (_powerUpPosXLabel) _powerUpPosXLabel->setVisible(false);
        if (_powerUpPosYInput) _powerUpPosYInput->setVisible(false);
        if (_powerUpPosYLabel) _powerUpPosYLabel->setVisible(false);
        if (_powerUpDeleteButton) _powerUpDeleteButton->setVisible(false);

        if (_editorModeDropdown) {
            _editorModeDropdown->setSelectedIndex(2);
        }

        const auto& sel = selection.value();
        const auto& wave = _waves[static_cast<size_t>(sel.waveIndex)];

        _currentWaveIndex = sel.waveIndex;
        if (_waveIndexLabel) {
            _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                " / " + std::to_string(_waves.size()));
            _waveIndexLabel->setVisible(true);
        }
        if (_wavePrevButton) {
            _wavePrevButton->setVisible(true);
        }
        if (_waveNextButton) {
            _waveNextButton->setVisible(true);
        }

        if (_waveTriggerXInput) {
            _waveTriggerXInput->setText(std::to_string(static_cast<int>(wave.gameXTrigger)));
            _waveTriggerXInput->setVisible(true);
        }
        if (_waveTriggerXLabel) {
            _waveTriggerXLabel->setVisible(true);
        }

        if (_waveDeleteButton) {
            _waveDeleteButton->setVisible(true);
        }
    } else {
        std::string editorMode = "";
        if (_editorModeDropdown) {
            editorMode = _editorModeDropdown->getSelectedOption();
        }

        if (editorMode == "Waves" && !_selectedWave.has_value()) {
            const float sidePanelWidth = 300.0f;
            float cursorMapX =
                _viewportOffset.getX() + (mouseX - sidePanelWidth) / _viewportZoom;

            Wave newWave;
            newWave.gameXTrigger = cursorMapX;
            _waves.push_back(newWave);

            _currentWaveIndex = static_cast<int>(_waves.size()) - 1;
            _selectedWave = WaveSelection{_currentWaveIndex, -1};

            if (_waveIndexLabel) {
                _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                    " / " + std::to_string(_waves.size()));
                _waveIndexLabel->setVisible(true);
            }
            if (_wavePrevButton) {
                _wavePrevButton->setVisible(true);
            }
            if (_waveNextButton) {
                _waveNextButton->setVisible(true);
            }
            if (_waveTriggerXInput) {
                _waveTriggerXInput->setText(std::to_string(static_cast<int>(
                    newWave.gameXTrigger)));
                _waveTriggerXInput->setVisible(true);
            }
            if (_waveTriggerXLabel) {
                _waveTriggerXLabel->setVisible(true);
            }
            if (_waveDeleteButton) {
                _waveDeleteButton->setVisible(true);
            }

            _hasUnsavedChanges = true;
            updateSaveButtonText();
        } else {
            _selectedWave = std::nullopt;

            if (_waveTriggerXInput) _waveTriggerXInput->setVisible(false);
            if (_waveTriggerXLabel) _waveTriggerXLabel->setVisible(false);
            if (_waveDeleteButton) _waveDeleteButton->setVisible(false);
            if (_waveIndexLabel) {
                _waveIndexLabel->setText("0 / " + std::to_string(_waves.size()));
            }
            if (_wavePrevButton) _wavePrevButton->setVisible(false);
            if (_waveNextButton) _waveNextButton->setVisible(false);
        }
    }
}

void LevelEditorState::startWaveDrag(
    math::Vector2f mousePos,
    float viewportZoom,
    float sidePanelWidth
) {
    if (!_selectedWave.has_value()) {
        _isDraggingWave = false;
        return;
    }

    const auto& sel = _selectedWave.value();
    if (sel.waveIndex < 0 || static_cast<size_t>(sel.waveIndex) >= _waves.size()) {
        _isDraggingWave = false;
        return;
    }

    float waveWorldX = _waves[static_cast<size_t>(sel.waveIndex)].gameXTrigger;

    float waveScreenX =
        sidePanelWidth + (waveWorldX - _viewportOffset.getX()) * viewportZoom;

    _dragWaveOffsetX = waveScreenX - mousePos.getX();
}

void LevelEditorState::handleWaveDrag(
    math::Vector2f mousePos,
    float viewportZoom,
    float sidePanelWidth
) {
    if (!_selectedWave.has_value()) {
        _isDraggingWave = false;
        return;
    }

    const auto& sel = _selectedWave.value();
    if (sel.waveIndex < 0 || static_cast<size_t>(sel.waveIndex) >= _waves.size()) {
        _isDraggingWave = false;
        return;
    }

    float newScreenX = mousePos.getX() + _dragWaveOffsetX;
    float newWorldX = _viewportOffset.getX() + (newScreenX - sidePanelWidth) / viewportZoom;

    _waves[static_cast<size_t>(sel.waveIndex)].gameXTrigger = newWorldX;

    if (_waveTriggerXInput) {
        _waveTriggerXInput->setText(std::to_string(static_cast<int>(newWorldX)));
    }

    _hasUnsavedChanges = true;
    updateSaveButtonText();
}

}  // namespace gsm
