/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorState
*/

#include "LevelEditorState.hpp"
#include <optional>
#include <filesystem>  // NOLINT(build/c++17)
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>
#include <nlohmann/json.hpp>
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"
#include "../../../../ui/elements/base/UIElement.hpp"

namespace gsm {

LevelEditorState::LevelEditorState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    std::optional<std::filesystem::path> levelPath
) : AGameState(gsm, resourceManager), _levelPath(levelPath) {
    if (_levelPath) {
        try {
            std::ifstream file(*_levelPath);
            file >> _levelData;
            file.close();
        } catch (const std::exception&) {
            if (auto stateMachine = gsm) {
                stateMachine->requestStatePop();
            }
            return;
        }
    } else {
        if (auto stateMachine = gsm) {
            stateMachine->requestStatePop();
        }
        return;
    }

    _history.push_back(_levelData);
    _currentHistoryIndex = 0;

    if (!_resourceManager->has<SettingsConfig>()) {
        _resourceManager->add(std::make_shared<SettingsConfig>());
    }

    _mouseHandler = std::make_unique<MouseInputHandler>(_resourceManager);
    _uiManager = std::make_unique<ui::UIManager>();
    _uiManager->setResourceManager(_resourceManager);

    _uiManager->setCursorCallback([this](bool isHovering) {
        if (_resourceManager->has<gfx::IWindow>()) {
            _resourceManager->get<gfx::IWindow>()->setCursor(isHovering);
        }
    });

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(6487.0f, 3584.0f));
    _uiManager->addElement(_background);

    createUI();
    initializeViewport();
}

void LevelEditorState::enter() {
    _uiManager->setOnBack([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });

    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(6487.0f, 3584.0f));

    _availableEnemies = loadAvailableEnemies();
    parseObstacles();
    parsePowerUps();
    parseWaves();
}

void LevelEditorState::update(float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (_uiManager->getGlobalScale() != config->getUIScale()) {
        _uiManager->setGlobalScale(config->getUIScale());
    }

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _uiManager->handleKeyboardInput(eventResult);

    bool ctrlPressed = _resourceManager->get<
        gfx::IEvent>()->isKeyPressed(gfx::EventType::LCTRL) ||
                       _resourceManager->get<
        gfx::IEvent>()->isKeyPressed(gfx::EventType::RCTRL);
    bool zPressed = _resourceManager->get<
        gfx::IEvent>()->isKeyPressed(gfx::EventType::Z);
    bool yPressed = _resourceManager->get<
        gfx::IEvent>()->isKeyPressed(gfx::EventType::Y);

    if (ctrlPressed && zPressed && !_undoPressedLastFrame) {
        if (_currentHistoryIndex > 0) {
            loadFromHistory(_currentHistoryIndex - 1);
        }
        _undoPressedLastFrame = true;
    } else if (!(ctrlPressed && zPressed)) {
        _undoPressedLastFrame = false;
    }

    if (ctrlPressed && yPressed && !_redoPressedLastFrame) {
        if (_currentHistoryIndex < _history.size() - 1) {
            loadFromHistory(_currentHistoryIndex + 1);
        }
        _redoPressedLastFrame = true;
    } else if (!(ctrlPressed && yPressed)) {
        _redoPressedLastFrame = false;
    }

    bool cPressed = _resourceManager->get<
        gfx::IEvent>()->isKeyPressed(gfx::EventType::C);
    bool vPressed = _resourceManager->get<
        gfx::IEvent>()->isKeyPressed(gfx::EventType::V);

    if (ctrlPressed && cPressed && !_copyPressedLastFrame) {
        if (_selectedObstacle.has_value()) {
            _clipboardObstacle = _selectedObstacle.value();
            _clipboardPowerUp = std::nullopt;
            _clipboardWave = std::nullopt;
        } else if (_selectedPowerUp.has_value()) {
            _clipboardPowerUp = _selectedPowerUp.value();
            _clipboardObstacle = std::nullopt;
            _clipboardWave = std::nullopt;
        } else if (_selectedWave.has_value()) {
            _clipboardWave = _selectedWave.value();
            _clipboardObstacle = std::nullopt;
            _clipboardPowerUp = std::nullopt;
        }
        _copyPressedLastFrame = true;
    } else if (!(ctrlPressed && cPressed)) {
        _copyPressedLastFrame = false;
    }

    if (ctrlPressed && vPressed && !_pastePressedLastFrame) {
        math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
        const float sidePanelWidth = 300.0f;
        auto coords = extractWorldCoordinates(mousePos, sidePanelWidth);

        if (_clipboardObstacle.has_value()) {
            if (_displayFilter != "All" && _displayFilter != "Obstacles") {
                _displayFilter = "All";
            }
            const auto& clipSel = _clipboardObstacle.value();
            if (clipSel.type == "unique") {
                auto& uniques = _obstaclesByName[clipSel.prefabName].uniques;
                if (clipSel.index < static_cast<int>(uniques.size())) {
                    auto newObstacle = uniques[static_cast<size_t>(clipSel.index)];
                    newObstacle.posX = coords.worldX;
                    newObstacle.posY = coords.worldY;
                    uniques.push_back(newObstacle);

                    ObstacleSelection newSel;
                    newSel.prefabName = clipSel.prefabName;
                    newSel.type = clipSel.type;
                    newSel.index = static_cast<int>(uniques.size()) - 1;
                    _selectedObstacle = newSel;

                    _selectedPowerUp = std::nullopt;
                    _selectedWave = std::nullopt;

                    if (_obstaclePosXInput) {
                        _obstaclePosXInput->setText(
                            std::to_string(static_cast<int>(coords.worldX)));
                    }
                    if (_obstaclePosYInput) {
                        _obstaclePosYInput->setText(
                            std::to_string(static_cast<int>(coords.worldY)));
                    }

                    showObstacleUI(false);
                    hidePowerUpUI();
                    hideWaveUI();
                    updateEnemyUI();

                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                    saveToHistory();
                }
            } else if (clipSel.type == "horizontal") {
                auto& horizontals = _obstaclesByName[clipSel.prefabName].horizontalLines;
                if (clipSel.index < static_cast<int>(horizontals.size())) {
                    auto newObstacle = horizontals[static_cast<size_t>(clipSel.index)];
                    newObstacle.fromX = coords.worldX;
                    newObstacle.posY = coords.worldY;
                    horizontals.push_back(newObstacle);

                    ObstacleSelection newSel;
                    newSel.prefabName = clipSel.prefabName;
                    newSel.type = clipSel.type;
                    newSel.index = static_cast<int>(horizontals.size()) - 1;
                    _selectedObstacle = newSel;

                    _selectedPowerUp = std::nullopt;
                    _selectedWave = std::nullopt;

                    if (_obstaclePosXInput) {
                        _obstaclePosXInput->setText(
                            std::to_string(static_cast<int>(coords.worldX)));
                    }
                    if (_obstaclePosYInput) {
                        _obstaclePosYInput->setText(
                            std::to_string(static_cast<int>(coords.worldY)));
                    }
                    if (_obstacleCountInput) {
                        _obstacleCountInput->setText(
                            std::to_string(newObstacle.count));
                    }

                    showObstacleUI(true);
                    hidePowerUpUI();
                    hideWaveUI();
                    updateEnemyUI();

                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                    saveToHistory();
                }
            } else if (clipSel.type == "vertical") {
                auto& verticals = _obstaclesByName[clipSel.prefabName].verticalLines;
                if (clipSel.index < static_cast<int>(verticals.size())) {
                    auto newObstacle = verticals[static_cast<size_t>(clipSel.index)];
                    newObstacle.posX = coords.worldX;
                    newObstacle.fromY = coords.worldY;
                    verticals.push_back(newObstacle);

                    ObstacleSelection newSel;
                    newSel.prefabName = clipSel.prefabName;
                    newSel.type = clipSel.type;
                    newSel.index = static_cast<int>(verticals.size()) - 1;
                    _selectedObstacle = newSel;

                    _selectedPowerUp = std::nullopt;
                    _selectedWave = std::nullopt;

                    if (_obstaclePosXInput) {
                        _obstaclePosXInput->setText(
                            std::to_string(static_cast<int>(coords.worldX)));
                    }
                    if (_obstaclePosYInput) {
                        _obstaclePosYInput->setText(
                            std::to_string(static_cast<int>(coords.worldY)));
                    }
                    if (_obstacleCountInput) {
                        _obstacleCountInput->setText(
                            std::to_string(newObstacle.count));
                    }

                    showObstacleUI(true);
                    hidePowerUpUI();
                    hideWaveUI();
                    updateEnemyUI();

                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                    saveToHistory();
                }
            }
        } else if (_clipboardPowerUp.has_value()) {
            if (_displayFilter != "All" && _displayFilter != "PowerUps") {
                _displayFilter = "All";
            }
            const auto& clipSel = _clipboardPowerUp.value();
            auto& powerUps = _powerUpsByName[clipSel.prefabName];
            if (clipSel.index < static_cast<int>(powerUps.size())) {
                PowerUpData newPowerUp;
                newPowerUp.posX = coords.worldX;
                newPowerUp.posY = coords.worldY;
                powerUps.push_back(newPowerUp);

                PowerUpSelection newSel;
                newSel.prefabName = clipSel.prefabName;
                newSel.index = static_cast<int>(powerUps.size()) - 1;
                _selectedPowerUp = newSel;

                _selectedObstacle = std::nullopt;
                _selectedWave = std::nullopt;

                if (_powerUpPosXInput) {
                    _powerUpPosXInput->setText(
                        std::to_string(static_cast<int>(coords.worldX)));
                }
                if (_powerUpPosYInput) {
                    _powerUpPosYInput->setText(
                        std::to_string(static_cast<int>(coords.worldY)));
                }

                showPowerUpUI();
                hideObstacleUI();
                hideWaveUI();
                updateEnemyUI();

                _hasUnsavedChanges = true;
                updateSaveButtonText();
                saveToHistory();
            }
        } else if (_clipboardWave.has_value()) {
            if (_displayFilter != "All" && _displayFilter != "Waves") {
                _displayFilter = "All";
            }
            const auto& clipSel = _clipboardWave.value();
            if (clipSel.waveIndex >= 0 &&
                clipSel.waveIndex < static_cast<int>(_waves.size())) {
                auto newWave = _waves[static_cast<size_t>(clipSel.waveIndex)];
                newWave.gameXTrigger = coords.worldX;
                _waves.push_back(newWave);

                WaveSelection newSel;
                newSel.waveIndex = static_cast<int>(_waves.size()) - 1;
                newSel.enemyIndex = -1;
                _selectedWave = newSel;
                _currentWaveIndex = newSel.waveIndex;

                _selectedObstacle = std::nullopt;
                _selectedPowerUp = std::nullopt;

                if (_waveIndexLabel) {
                    _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                        " / " + std::to_string(_waves.size()));
                }
                if (_waveTriggerXInput) {
                    _waveTriggerXInput->setText(
                        std::to_string(static_cast<int>(coords.worldX)));
                }

                showWaveUI();
                hideObstacleUI();
                hidePowerUpUI();
                updateEnemyUI();

                _hasUnsavedChanges = true;
                updateSaveButtonText();
                saveToHistory();
            }
        }
        _pastePressedLastFrame = true;
    } else if (!(ctrlPressed && vPressed)) {
        _pastePressedLastFrame = false;
    }

    if (eventResult == gfx::EventType::TEXT_INPUT) {
        std::string textInput = _resourceManager->get<gfx::IEvent>()->getLastTextInput();
        if (!textInput.empty()) {
            _uiManager->handleTextInput(textInput);
        }
    }

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool mousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    _uiManager->handleMouseInput(mousePos, mousePressed);

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        _uiManager->handleNavigationInputs(inputProvider, deltaTime);
    }

    _uiManager->update(deltaTime);

    handleZoom(deltaTime, eventResult);
    handleCanvasDrag(deltaTime);

    for (auto& [prefabName, spriteData] : _obstacleAnimationData) {
        if (spriteData.isAnimation) {
            if (_obstacleAnimationTimes.find(prefabName) == _obstacleAnimationTimes.end()) {
                _obstacleAnimationTimes[prefabName] = 0.0f;
                _obstacleAnimationFrames[prefabName] = 0.0f;
            }

            _obstacleAnimationTimes[prefabName] += deltaTime;
            float frameDuration = spriteData.animationSpeed;

            if (_obstacleAnimationTimes[prefabName] >= frameDuration) {
                _obstacleAnimationTimes[prefabName] = 0.0f;
                _obstacleAnimationFrames[prefabName] += 1.0f;

                if (_obstacleAnimationFrames[prefabName] >= spriteData.frameCount) {
                    if (spriteData.animationLoop) {
                        _obstacleAnimationFrames[prefabName] = 0.0f;
                    } else {
                        _obstacleAnimationFrames[prefabName] = spriteData.frameCount - 1.0f;
                    }
                }
            }
        }
    }

    for (auto& [prefabName, spriteData] : _powerUpAnimationData) {
        if (spriteData.isAnimation) {
            if (_powerUpAnimationTimes.find(prefabName) == _powerUpAnimationTimes.end()) {
                _powerUpAnimationTimes[prefabName] = 0.0f;
                _powerUpAnimationFrames[prefabName] = 0.0f;
            }

            _powerUpAnimationTimes[prefabName] += deltaTime;
            float frameDuration = spriteData.animationSpeed;

            if (_powerUpAnimationTimes[prefabName] >= frameDuration) {
                _powerUpAnimationTimes[prefabName] = 0.0f;
                _powerUpAnimationFrames[prefabName] += 1.0f;

                if (_powerUpAnimationFrames[prefabName] >= spriteData.frameCount) {
                    if (spriteData.animationLoop) {
                        _powerUpAnimationFrames[prefabName] = 0.0f;
                    } else {
                        _powerUpAnimationFrames[prefabName] = spriteData.frameCount - 1.0f;
                    }
                }
            }
        }
    }

    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    bool isInCanvas = mousePos.getX() >= sidePanelWidth &&
                      mousePos.getX() <= constants::MAX_WIDTH &&
                      mousePos.getY() >= 0.0f &&
                      mousePos.getY() <= canvasHeight;

    bool leftMousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::LEFT));

    if (isInCanvas && !leftMousePressed && _leftMousePressedLastFrame &&
        !_isDragging && !_isDraggingObstacle && !_isDraggingPowerUp) {
        float mapLength = _levelData.value(constants::MAP_LENGTH_FIELD, 0.0f);
        if (mapLength > 0.0f) {
            float levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
            float levelY = -(_viewportOffset.getY() * _viewportZoom);

            std::string editorMode = "";
            if (_editorModeDropdown) {
                editorMode = _editorModeDropdown->getSelectedOption();
            }

            if (editorMode == "Obstacles") {
                auto obstacleSelection = getObstacleAtPosition(
                    mousePos.getX(), mousePos.getY(), levelX, levelY);
                if (obstacleSelection.has_value() && (_displayFilter == "All" ||
                    _displayFilter == "Obstacles")) {
                    handleObstacleClick(mousePos.getX(), mousePos.getY(), levelX, levelY);
                } else {
                    auto powerUpSelection = getPowerUpAtPosition(
                        mousePos.getX(), mousePos.getY(), levelX, levelY);
                    if (powerUpSelection.has_value() && (_displayFilter == "All" ||
                        _displayFilter == "PowerUps")) {
                        handlePowerUpClick(mousePos.getX(), mousePos.getY(), levelX, levelY);
                    } else {
                        auto waveSelection = getWaveAtPosition(
                            mousePos.getX(), mousePos.getY(), levelX, levelY);
                        if (waveSelection.has_value() && (_displayFilter == "All" ||
                            _displayFilter == "Waves")) {
                            handleWaveClick(
                                mousePos.getX(), mousePos.getY(), levelX, levelY);
                        } else {
                            handleObstacleClick(
                                mousePos.getX(), mousePos.getY(), levelX, levelY);
                        }
                    }
                }
            } else if (editorMode == "PowerUps") {
                auto powerUpSelection = getPowerUpAtPosition(
                    mousePos.getX(), mousePos.getY(), levelX, levelY);
                if (powerUpSelection.has_value() && (_displayFilter == "All" ||
                        _displayFilter == "PowerUps")) {
                    handlePowerUpClick(mousePos.getX(), mousePos.getY(), levelX, levelY);
                } else {
                    auto obstacleSelection = getObstacleAtPosition(
                        mousePos.getX(), mousePos.getY(), levelX, levelY);
                    if (obstacleSelection.has_value() && (_displayFilter == "All" ||
                            _displayFilter == "Obstacles")) {
                        handleObstacleClick(
                            mousePos.getX(), mousePos.getY(), levelX, levelY);
                    } else {
                        auto waveSelection = getWaveAtPosition(
                            mousePos.getX(), mousePos.getY(), levelX, levelY);
                        if (waveSelection.has_value() && (_displayFilter == "All" ||
                            _displayFilter == "Waves")) {
                            handleWaveClick(
                                mousePos.getX(), mousePos.getY(), levelX, levelY);
                        } else {
                            handlePowerUpClick(
                                mousePos.getX(), mousePos.getY(), levelX, levelY);
                        }
                    }
                }
            } else if (editorMode == "Waves") {
                auto waveSelection = getWaveAtPosition(
                    mousePos.getX(), mousePos.getY(), levelX, levelY);
                if (waveSelection.has_value() && (_displayFilter == "All" ||
                        _displayFilter == "Waves")) {
                    handleWaveClick(mousePos.getX(), mousePos.getY(), levelX, levelY);
                } else {
                    auto obstacleSelection = getObstacleAtPosition(
                        mousePos.getX(), mousePos.getY(), levelX, levelY);
                    if (obstacleSelection.has_value() && (_displayFilter == "All" ||
                        _displayFilter == "Obstacles")) {
                        handleObstacleClick(mousePos.getX(), mousePos.getY(), levelX, levelY);
                    } else {
                        auto powerUpSelection = getPowerUpAtPosition(
                            mousePos.getX(), mousePos.getY(), levelX, levelY);
                        if (powerUpSelection.has_value() && (_displayFilter == "All" ||
                            _displayFilter == "PowerUps")) {
                            handlePowerUpClick(
                                mousePos.getX(), mousePos.getY(), levelX, levelY);
                        } else {
                            handleWaveClick(mousePos.getX(), mousePos.getY(), levelX, levelY);
                        }
                    }
                }
            }
        }
    }

    if (leftMousePressed && isInCanvas) {
        if (_selectedObstacle.has_value()) {
            float mapLength = _levelData.value(constants::MAP_LENGTH_FIELD, 0.0f);
            if (mapLength > 0.0f) {
                if (_isDraggingObstacle) {
                    handleObstacleDrag(mousePos, _viewportZoom, sidePanelWidth);
                } else {
                    float levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
                    float levelY = -(_viewportOffset.getY() * _viewportZoom);
                    auto clickedObstacle = getObstacleAtPosition(
                        mousePos.getX(), mousePos.getY(), levelX, levelY);

                    if (clickedObstacle.has_value() &&
                        clickedObstacle.value().prefabName ==
                            _selectedObstacle.value().prefabName &&
                        clickedObstacle.value().type == _selectedObstacle.value().type &&
                        clickedObstacle.value().index == _selectedObstacle.value().index) {
                        _isDraggingObstacle = true;
                        startObstacleDrag(mousePos, _viewportZoom, sidePanelWidth);
                    }
                }
            }
        }
        if (_selectedPowerUp.has_value()) {
            float mapLength = _levelData.value(constants::MAP_LENGTH_FIELD, 0.0f);
            if (mapLength > 0.0f) {
                if (_isDraggingPowerUp) {
                    handlePowerUpDrag(mousePos, _viewportZoom, sidePanelWidth);
                } else {
                    float levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
                    float levelY = -(_viewportOffset.getY() * _viewportZoom);
                    auto clickedPowerUp = getPowerUpAtPosition(
                        mousePos.getX(), mousePos.getY(), levelX, levelY);

                    if (clickedPowerUp.has_value() &&
                        clickedPowerUp.value().prefabName ==
                            _selectedPowerUp.value().prefabName &&
                        clickedPowerUp.value().index == _selectedPowerUp.value().index) {
                        _isDraggingPowerUp = true;
                        startPowerUpDrag(mousePos, _viewportZoom, sidePanelWidth);
                    }
                }
            }
        }
        if (_selectedWave.has_value()) {
            float mapLength = _levelData.value(constants::MAP_LENGTH_FIELD, 0.0f);
            if (mapLength > 0.0f) {
                if (_isDraggingWave) {
                    handleWaveDrag(mousePos, _viewportZoom, sidePanelWidth);
                } else {
                    float levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
                    float levelY = -(_viewportOffset.getY() * _viewportZoom);
                    auto clickedWave = getWaveAtPosition(
                        mousePos.getX(), mousePos.getY(), levelX, levelY);

                    if (clickedWave.has_value() &&
                        clickedWave.value().waveIndex == _selectedWave.value().waveIndex) {
                        _isDraggingWave = true;
                        startWaveDrag(mousePos, _viewportZoom, sidePanelWidth);
                    }
                }
            }
        }
    } else {
        _isDraggingObstacle = false;
        _isDraggingPowerUp = false;
        _isDraggingWave = false;
    }

    _leftMousePressedLastFrame = leftMousePressed;

    if (isInCanvas) {
        float cursorMapX =
            _viewportOffset.getX() + (mousePos.getX() - sidePanelWidth) / _viewportZoom;
        float cursorMapY =
            _viewportOffset.getY() + mousePos.getY() / _viewportZoom;

        std::stringstream ssX;
        ssX << " Level X:  " << static_cast<int>(cursorMapX);
        _cursorPosLabel->setText(ssX.str());

        std::stringstream ssY;
        ssY << " Level Y:  " << static_cast<int>(cursorMapY);
        _cursorPosYLabel->setText(ssY.str());
    } else {
        _cursorPosLabel->setText(" Level X:  N/A");
        _cursorPosYLabel->setText(" Level Y:  N/A");
    }

    renderUI();

    if (_hasPendingChange) {
        _lastChangeTime += deltaTime;
        if (_lastChangeTime >= _currentDebounceTime) {
            saveToHistory();
            _hasPendingChange = false;
            _lastChangeTime = 0.0f;
        }
    }
}

void LevelEditorState::exit() {
    auto window = _resourceManager->get<gfx::IWindow>();
    window->setCursor(false);
    _uiManager->clearElements();
    _background.reset();
    _sidePanel.reset();
    _bottomPanel.reset();
    _canvasPanel.reset();
    _spritePreviewPanel.reset();
    _spritePreview.reset();
    _spriteWidthLabel.reset();
    _spriteHeightLabel.reset();
    _saveButton.reset();
    _backButton.reset();
    _nameLabel.reset();
    _levelNameInput.reset();
    _mapLengthLabel.reset();
    _mapLengthInput.reset();
    _scrollSpeedLabel.reset();
    _scrollSpeedInput.reset();
    _musicLabel.reset();
    _musicDropdown.reset();
    _backgroundLabel.reset();
    _backgroundDropdown.reset();
    _undoButton.reset();
    _redoButton.reset();
    _cursorPosLabel.reset();
    _cursorPosYLabel.reset();
    _resetViewButton.reset();
    _showHitboxesButton.reset();
    _editorModeDropdown.reset();
    _obstaclePrefabLabel.reset();
    _obstaclePrefabDropdown.reset();
    _obstacleTypeLabel.reset();
    _obstacleTypeDropdown.reset();
    _obstaclePosXLabel.reset();
    _obstaclePosXInput.reset();
    _obstaclePosYLabel.reset();
    _obstaclePosYInput.reset();
    _obstacleCountLabel.reset();
    _obstacleCountInput.reset();
    _obstacleDeleteButton.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

}  // namespace gsm
