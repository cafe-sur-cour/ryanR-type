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

    auto config = _resourceManager->get<SettingsConfig>();
    _uiManager->setGlobalScale(config->getUIScale());

    _background = std::make_shared<ui::Background>(_resourceManager);
    _background->addLayer(constants::UI_BACKGROUND_EARTH_PATH, 0.0f, 0.0f,
        math::Vector2f(5376.0f, 3584.0f));
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
        math::Vector2f(5376.0f, 3584.0f));

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

    bool isHoveringUI = _uiManager->isMouseHoveringAnyElement(mousePos);
    _resourceManager->get<gfx::IWindow>()->setCursor(isHoveringUI);

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
        float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);
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
            float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);
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
            float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);
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
            float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);
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

void LevelEditorState::renderUI() {
    _uiManager->render();
    renderLevelPreview();
}

std::vector<std::string> LevelEditorState::loadAvailableMusics() {
    std::vector<std::string> musics;
    std::filesystem::path musicsPath = constants::MUSIC_DIRECTORY;

    if (!std::filesystem::exists(musicsPath) || !std::filesystem::is_directory(musicsPath)) {
        return musics;
    }

    for (const auto& entry : std::filesystem::directory_iterator(musicsPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json musicData;
                file >> musicData;
                file.close();

                if (musicData.contains(constants::LEVEL_NAME_FIELD) &&
                    musicData[constants::LEVEL_NAME_FIELD].is_string()) {
                    std::string musicName = musicData[constants::LEVEL_NAME_FIELD];
                    musics.push_back(musicName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return musics;
}

std::vector<std::string> LevelEditorState::loadAvailableBackgrounds() {
    std::vector<std::string> backgrounds;
    std::filesystem::path backgroundsPath = constants::BACKGROUNDS_DIRECTORY;

    if (!std::filesystem::exists(backgroundsPath) ||
        !std::filesystem::is_directory(backgroundsPath)) {
        return backgrounds;
    }

    for (const auto& entry : std::filesystem::directory_iterator(backgroundsPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json backgroundData;
                file >> backgroundData;
                file.close();

                if (backgroundData.contains(constants::LEVEL_NAME_FIELD) &&
                    backgroundData[constants::LEVEL_NAME_FIELD].is_string()) {
                    std::string backgroundName = backgroundData[constants::LEVEL_NAME_FIELD];
                    backgrounds.push_back(backgroundName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return backgrounds;
}

std::vector<std::string> LevelEditorState::loadAvailableObstacles() {
    std::vector<std::string> obstacles;
    std::filesystem::path obstaclesPath = constants::OBSTACLES_DIRECTORY;

    if (!std::filesystem::exists(obstaclesPath) ||
        !std::filesystem::is_directory(obstaclesPath)) {
        return obstacles;
    }

    for (const auto& entry : std::filesystem::directory_iterator(obstaclesPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json obstacleData;
                file >> obstacleData;
                file.close();

                if (obstacleData.contains(constants::LEVEL_NAME_FIELD) &&
                    obstacleData[constants::LEVEL_NAME_FIELD].is_string()) {
                    std::string obstacleName = obstacleData[constants::LEVEL_NAME_FIELD];
                    obstacles.push_back(obstacleName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return obstacles;
}

std::vector<std::string> LevelEditorState::loadAvailablePowerUps() {
    std::vector<std::string> powerUps;
    std::filesystem::path powerUpsPath = constants::POWERUPS_DIRECTORY;

    if (!std::filesystem::exists(powerUpsPath) ||
        !std::filesystem::is_directory(powerUpsPath)) {
        return powerUps;
    }

    for (const auto& entry : std::filesystem::directory_iterator(powerUpsPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json powerUpData;
                file >> powerUpData;
                file.close();

                if (powerUpData.contains(constants::LEVEL_NAME_FIELD) &&
                    powerUpData[constants::LEVEL_NAME_FIELD].is_string()) {
                    std::string powerUpName = powerUpData[constants::LEVEL_NAME_FIELD];
                    powerUps.push_back(powerUpName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return powerUps;
}

std::vector<std::string> LevelEditorState::loadAvailableEnemies() {
    std::vector<std::string> enemies;
    std::filesystem::path enemiesPath = constants::ENEMIES_DIRECTORY;

    if (!std::filesystem::exists(enemiesPath) ||
        !std::filesystem::is_directory(enemiesPath)) {
        return enemies;
    }

    for (const auto& entry : std::filesystem::directory_iterator(enemiesPath)) {
        if (entry.is_regular_file() && entry.path().extension() ==
            constants::LEVEL_FILE_EXTENSION) {
            try {
                std::ifstream file(entry.path());
                nlohmann::json enemyData;
                file >> enemyData;
                file.close();

                if (enemyData.contains(constants::LEVEL_NAME_FIELD) &&
                    enemyData[constants::LEVEL_NAME_FIELD].is_string()) {
                    std::string enemyName = enemyData[constants::LEVEL_NAME_FIELD];
                    enemies.push_back(enemyName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return enemies;
}

void LevelEditorState::createUI() {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasWidth = constants::MAX_WIDTH - sidePanelWidth;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    _sidePanel = std::make_shared<ui::Panel>(_resourceManager);
    _sidePanel->setPosition(math::Vector2f(0.0f, 0.0f));
    _sidePanel->setSize(math::Vector2f(sidePanelWidth, constants::MAX_HEIGHT));
    _sidePanel->setBackgroundColor(colors::LIGHT_GRAY);
    _sidePanel->setBorderColor(colors::GRAY);
    _uiManager->addElement(_sidePanel);

    _saveButton = std::make_shared<ui::Button>(_resourceManager);
    _saveButton->setPosition(math::Vector2f(10.0f, 15.0f));
    _saveButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _saveButton->setText("Save Level");
    _saveButton->setOnClick([this]() {
        if (!_levelPath || !validateFields()) {
            return;
        }

        std::string levelName = _levelNameInput->getText();
        std::string mapLengthStr = _mapLengthInput->getText();
        std::string scrollSpeedStr = _scrollSpeedInput->getText();

        _levelData[constants::LEVEL_NAME_FIELD] = levelName;

        try {
            float mapLength = std::stof(mapLengthStr);
            _levelData[constants::LEVEL_MAP_LENGTH_FIELD] = mapLength;
        } catch (const std::exception&) {
        }

        try {
            int scrollSpeed = std::stoi(scrollSpeedStr);
            _levelData[constants::LEVEL_SCROLL_SPEED_FIELD] = scrollSpeed;
        } catch (const std::exception&) {
        }

        saveObstacles();
        savePowerUps();
        saveWaves();

        std::filesystem::path savePath = *_levelPath;

        std::ofstream file(savePath);
        if (file.is_open()) {
            file << _levelData.dump(4);
            file.close();
            _hasUnsavedChanges = false;
            updateSaveButtonText();
        }
    });
    _saveButton->setScalingEnabled(false);
    _saveButton->setFocusEnabled(true);
    _sidePanel->addChild(_saveButton);

    _backButton = std::make_shared<ui::Button>(_resourceManager);
    _backButton->setPosition(math::Vector2f(10.0f, 70.0f));
    _backButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _backButton->setText("Back");
    _backButton->setNormalColor(colors::BUTTON_SECONDARY);
    _backButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _backButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _backButton->setOnClick([this]() {
        if (auto stateMachine = this->_gsm.lock()) {
            stateMachine->requestStatePop();
        }
    });
    _backButton->setScalingEnabled(false);
    _backButton->setFocusEnabled(true);
    _sidePanel->addChild(_backButton);

    float currentY = 150.0f;
    const float elementSpacing = 20.0f;
    const float labelToFieldSpacing = 40.0f;

    _nameLabel = std::make_shared<ui::Text>(_resourceManager);
    _nameLabel->setPosition(math::Vector2f(10.0f, currentY));
    _nameLabel->setText("Name");
    _nameLabel->setFontSize(24);
    _nameLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_nameLabel);

    currentY += labelToFieldSpacing;
    _levelNameInput = std::make_shared<ui::TextInput>(_resourceManager);
    _levelNameInput->setPosition(math::Vector2f(10.0f, currentY));
    _levelNameInput->setSize(math::Vector2f(sidePanelWidth - 25.0f, 30.0f));
    _levelNameInput->setText(_levelData.value(constants::LEVEL_NAME_FIELD, "New Level"));
    _levelNameInput->setPlaceholder("Enter level name...");
    _levelNameInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory) {
            return;
        }
        _levelData[constants::LEVEL_NAME_FIELD] = text;
        _hasPendingChange = true;
        _lastChangeTime = 0.0f;
        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
    });
    _levelNameInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_levelNameInput);
    });
    _levelNameInput->setScalingEnabled(false);
    _levelNameInput->setFocusEnabled(true);
    _sidePanel->addChild(_levelNameInput);

    currentY += 40.0f + elementSpacing;
    _mapLengthLabel = std::make_shared<ui::Text>(_resourceManager);
    _mapLengthLabel->setPosition(math::Vector2f(10.0f, currentY));
    _mapLengthLabel->setText("Map Length");
    _mapLengthLabel->setFontSize(24);
    _mapLengthLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_mapLengthLabel);

    currentY += labelToFieldSpacing;
    _mapLengthInput = std::make_shared<ui::TextInput>(_resourceManager);
    _mapLengthInput->setPosition(math::Vector2f(10.0f, currentY));
    _mapLengthInput->setSize(math::Vector2f(sidePanelWidth - 25.0f, 30.0f));
    _mapLengthInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0))));
    _mapLengthInput->setPlaceholder("Enter map length...");
    _mapLengthInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory) {
            return;
        }

        std::string filteredText;
        for (char c : text) {
            if (c >= '0' && c <= '9') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _mapLengthInput->setText(filteredText);
            return;
        }

        try {
            float mapLength = std::stof(filteredText);
            _levelData[constants::LEVEL_MAP_LENGTH_FIELD] = mapLength;
            _hasPendingChange = true;
            _lastChangeTime = 0.0f;
        } catch (const std::exception&) {
        }

        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
    });
    _mapLengthInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_mapLengthInput);
    });
    _mapLengthInput->setScalingEnabled(false);
    _mapLengthInput->setFocusEnabled(true);
    _sidePanel->addChild(_mapLengthInput);

    currentY += 40.0f + elementSpacing;
    _scrollSpeedLabel = std::make_shared<ui::Text>(_resourceManager);
    _scrollSpeedLabel->setPosition(math::Vector2f(10.0f, currentY));
    _scrollSpeedLabel->setText("Scroll Speed");
    _scrollSpeedLabel->setFontSize(24);
    _scrollSpeedLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_scrollSpeedLabel);

    currentY += labelToFieldSpacing;
    _scrollSpeedInput = std::make_shared<ui::TextInput>(_resourceManager);
    _scrollSpeedInput->setPosition(math::Vector2f(10.0f, currentY));
    _scrollSpeedInput->setSize(math::Vector2f(sidePanelWidth - 25.0f, 30.0f));
    _scrollSpeedInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::LEVEL_SCROLL_SPEED_FIELD, 1))));
    _scrollSpeedInput->setPlaceholder("Enter scroll speed...");
    _scrollSpeedInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory) {
            return;
        }

        std::string filteredText;
        for (char c : text) {
            if (c >= '0' && c <= '9') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _scrollSpeedInput->setText(filteredText);
            return;
        }

        try {
            int scrollSpeed = std::stoi(filteredText);
            _levelData[constants::LEVEL_SCROLL_SPEED_FIELD] = scrollSpeed;
            _hasPendingChange = true;
            _lastChangeTime = 0.0f;
        } catch (const std::exception&) {
        }

        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
    });
    _scrollSpeedInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_scrollSpeedInput);
    });
    _scrollSpeedInput->setScalingEnabled(false);
    _scrollSpeedInput->setFocusEnabled(true);
    _sidePanel->addChild(_scrollSpeedInput);

    currentY += 40.0f + elementSpacing;
    _musicLabel = std::make_shared<ui::Text>(_resourceManager);
    _musicLabel->setPosition(math::Vector2f(10.0f, currentY));
    _musicLabel->setText("Music");
    _musicLabel->setFontSize(24);
    _musicLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_musicLabel);

    currentY += labelToFieldSpacing;
    _musicDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _musicDropdown->setPosition(math::Vector2f(10.0f, currentY));
    _musicDropdown->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    auto availableMusics = loadAvailableMusics();
    _musicDropdown->setOptions(availableMusics);
    _musicDropdown->setPlaceholder("Select music...");

    std::string currentMusic = _levelData.value(constants::LEVEL_MUSIC_FIELD, "");
    if (!currentMusic.empty()) {
        auto options = _musicDropdown->getOptions();
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == currentMusic) {
                _musicDropdown->setSelectedIndex(i);
                break;
            }
        }
    }

    _musicDropdown->setOnSelectionChanged(
        [this](const std::string& musicName, [[maybe_unused]] size_t index) {
        if (_isLoadingFromHistory) {
            return;
        }
        _levelData[constants::LEVEL_MUSIC_FIELD] = musicName;
        saveToHistory();
        _hasUnsavedChanges = true;
        updateSaveButtonText();
    });
    _musicDropdown->setScalingEnabled(false);
    _musicDropdown->setFocusEnabled(true);

    currentY += 40.0f + elementSpacing;
    _backgroundLabel = std::make_shared<ui::Text>(_resourceManager);
    _backgroundLabel->setPosition(math::Vector2f(10.0f, currentY));
    _backgroundLabel->setText("Background");
    _backgroundLabel->setFontSize(24);
    _backgroundLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_backgroundLabel);

    currentY += labelToFieldSpacing;
    _backgroundDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _backgroundDropdown->setPosition(math::Vector2f(10.0f, currentY));
    _backgroundDropdown->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    auto availableBackgrounds = loadAvailableBackgrounds();
    _backgroundDropdown->setOptions(availableBackgrounds);
    _backgroundDropdown->setPlaceholder("Select background...");

    std::string currentBackground = _levelData.value(constants::LEVEL_BACKGROUND_FIELD, "");
    if (!currentBackground.empty()) {
        auto options = _backgroundDropdown->getOptions();
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == currentBackground) {
                _backgroundDropdown->setSelectedIndex(i);
                break;
            }
        }
    }

    _backgroundDropdown->setOnSelectionChanged(
        [this](const std::string& backgroundName, [[maybe_unused]] size_t index) {
        if (_isLoadingFromHistory) {
            return;
        }
        _levelData[constants::LEVEL_BACKGROUND_FIELD] = backgroundName;
        saveToHistory();
        _hasUnsavedChanges = true;
        updateSaveButtonText();
    });
    _backgroundDropdown->setScalingEnabled(false);
    _backgroundDropdown->setFocusEnabled(true);

    currentY += 60.0f + elementSpacing;
    _showHitboxesButton = std::make_shared<ui::Button>(_resourceManager);
    _showHitboxesButton->setPosition(math::Vector2f(10.0f, currentY));
    _showHitboxesButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _showHitboxesButton->setText("Hitboxes: Hide");
    _showHitboxesButton->setNormalColor(colors::BUTTON_SECONDARY);
    _showHitboxesButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _showHitboxesButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _showHitboxesButton->setOnRelease([this]() {
        _showHitboxes = !_showHitboxes;
        std::string text = "Hitboxes: " + std::string(_showHitboxes ? "Show" : "Hide");
        _showHitboxesButton->setText(text);
    });
    _showHitboxesButton->setScalingEnabled(false);
    _showHitboxesButton->setFocusEnabled(true);
    _sidePanel->addChild(_showHitboxesButton);

    currentY += 60.0f;
    _filterButton = std::make_shared<ui::Button>(_resourceManager);
    _filterButton->setPosition(math::Vector2f(10.0f, currentY));
    _filterButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _filterButton->setText("Show: All");
    _filterButton->setNormalColor(colors::BUTTON_SECONDARY);
    _filterButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _filterButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _filterButton->setOnRelease([this]() {
        if (_displayFilter == "All") {
            _displayFilter = "Obstacles";
        } else if (_displayFilter == "Obstacles") {
            _displayFilter = "PowerUps";
        } else if (_displayFilter == "PowerUps") {
            _displayFilter = "Waves";
        } else {
            _displayFilter = "All";
        }
        _filterButton->setText("Show: " + _displayFilter);
    });
    _filterButton->setScalingEnabled(false);
    _filterButton->setFocusEnabled(true);
    _sidePanel->addChild(_filterButton);

    _sidePanel->addChild(_backgroundDropdown);
    _sidePanel->addChild(_musicDropdown);

    const float buttonWidth = (sidePanelWidth - 35.0f) / 2.0f;
    const float buttonY = constants::MAX_HEIGHT - 60.0f;

    _cursorPosLabel = std::make_shared<ui::Text>(_resourceManager);
    _cursorPosLabel->setPosition(math::Vector2f(10.0f, buttonY - 150.0f));
    _cursorPosLabel->setText(" Level X:  0");
    _cursorPosLabel->setFontSize(28);
    _cursorPosLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_cursorPosLabel);

    _cursorPosYLabel = std::make_shared<ui::Text>(_resourceManager);
    _cursorPosYLabel->setPosition(math::Vector2f(10.0f, buttonY - 110.0f));
    _cursorPosYLabel->setText(" Level Y:  0");
    _cursorPosYLabel->setFontSize(28);
    _cursorPosYLabel->setTextColor(colors::BUTTON_PRIMARY);
    _sidePanel->addChild(_cursorPosYLabel);

    _resetViewButton = std::make_shared<ui::Button>(_resourceManager);
    _resetViewButton->setPosition(math::Vector2f(10.0f, buttonY - 65.0f));
    _resetViewButton->setSize(math::Vector2f(sidePanelWidth - 25.0f, 40.0f));
    _resetViewButton->setText("Reset View");
    _resetViewButton->setNormalColor(colors::BUTTON_SECONDARY);
    _resetViewButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _resetViewButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _resetViewButton->setOnRelease([this]() {
        initializeViewport();
    });
    _resetViewButton->setScalingEnabled(false);
    _resetViewButton->setFocusEnabled(true);
    _sidePanel->addChild(_resetViewButton);

    _undoButton = std::make_shared<ui::Button>(_resourceManager);
    _undoButton->setPosition(math::Vector2f(10.0f, buttonY));
    _undoButton->setSize(math::Vector2f(buttonWidth, 40.0f));
    _undoButton->setText("Undo");
    _undoButton->setNormalColor(colors::BUTTON_SECONDARY);
    _undoButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _undoButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _undoButton->setOnRelease([this]() {
        if (_currentHistoryIndex > 0) {
            loadFromHistory(_currentHistoryIndex - 1);
        }
    });
    _undoButton->setScalingEnabled(false);
    _undoButton->setFocusEnabled(true);
    _sidePanel->addChild(_undoButton);

    _redoButton = std::make_shared<ui::Button>(_resourceManager);
    _redoButton->setPosition(math::Vector2f(20.0f + buttonWidth, buttonY));
    _redoButton->setSize(math::Vector2f(buttonWidth, 40.0f));
    _redoButton->setText("Redo");
    _redoButton->setNormalColor(colors::BUTTON_SECONDARY);
    _redoButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _redoButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _redoButton->setOnRelease([this]() {
        if (_currentHistoryIndex < _history.size() - 1) {
            loadFromHistory(_currentHistoryIndex + 1);
        }
    });
    _redoButton->setScalingEnabled(false);
    _redoButton->setFocusEnabled(true);
    _sidePanel->addChild(_redoButton);

    updateHistoryButtons();

    createBottomPanel();

    _canvasPanel = std::make_shared<ui::Panel>(_resourceManager);
    _canvasPanel->setPosition(math::Vector2f(sidePanelWidth, 0.0f));
    _canvasPanel->setSize(math::Vector2f(canvasWidth, canvasHeight));
    _canvasPanel->setBackgroundColor(colors::BLACK);
    _canvasPanel->setBorderColor(colors::GRAY);
    _uiManager->addElement(_canvasPanel);

    _saveButton->setState(validateFields() ? ui::UIState::Normal : ui::UIState::Disabled);
}

void LevelEditorState::createBottomPanel() {
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

    _obstaclePrefabLabel = std::make_shared<ui::Text>(_resourceManager);
    _obstaclePrefabLabel->setPosition(math::Vector2f(10.0f, 100.0f));
    _obstaclePrefabLabel->setText("Prefab");
    _obstaclePrefabLabel->setFontSize(16);
    _obstaclePrefabLabel->setTextColor(colors::BUTTON_PRIMARY);
    _obstaclePrefabLabel->setVisible(true);
    _bottomPanel->addChild(_obstaclePrefabLabel);

    _obstaclePrefabDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _obstaclePrefabDropdown->setPosition(math::Vector2f(10.0f, 130.0f));
    _obstaclePrefabDropdown->setSize(math::Vector2f(200.0f, 40.0f));
    auto availableObstacles = loadAvailableObstacles();
    _obstaclePrefabDropdown->setOptions(availableObstacles);
    _obstaclePrefabDropdown->setPlaceholder("Prefab...");
    _obstaclePrefabDropdown->setOnSelectionChanged(
        [this](const std::string& obstacle, [[maybe_unused]] size_t index) {
        if (!obstacle.empty()) {
            std::filesystem::path prefabPath =
                std::filesystem::path(constants::OBSTACLES_DIRECTORY) / (obstacle +
                    constants::LEVEL_FILE_EXTENSION);
            if (_spritePreview && std::filesystem::exists(prefabPath)) {
                _spritePreview->loadPrefab(prefabPath);

                LevelPreviewSprite spriteData =
                    extractSpriteDataFromPrefab(prefabPath.string());
                std::string widthStr = "Width: " +
                    std::to_string(static_cast<int>(spriteData.width));
                std::string heightStr = "Height: " +
                    std::to_string(static_cast<int>(spriteData.height));
                _spriteWidthLabel->setText(widthStr);
                _spriteHeightLabel->setText(heightStr);

                _obstacleAnimationData[obstacle] = spriteData;
                if (spriteData.isAnimation) {
                    _obstacleAnimationFrames[obstacle] = 0.0f;
                    _obstacleAnimationTimes[obstacle] = 0.0f;
                }
            }
        }
    });
    _obstaclePrefabDropdown->setScalingEnabled(false);
    _obstaclePrefabDropdown->setFocusEnabled(true);
    _obstaclePrefabDropdown->setDirection(ui::DropdownDirection::Right);
    _obstaclePrefabDropdown->setVisible(true);

    const float previewPanelSize = 180.0f;
    _spritePreviewPanel = std::make_shared<ui::Panel>(_resourceManager);
    _spritePreviewPanel->setPosition(math::Vector2f(250.0f, 12.5f));
    _spritePreviewPanel->setSize(math::Vector2f(previewPanelSize, previewPanelSize));
    _spritePreviewPanel->setBackgroundColor(colors::WHITE);
    _spritePreviewPanel->setBorderColor(colors::GRAY);
    _spritePreviewPanel->setVisible(true);
    _bottomPanel->addChild(_spritePreviewPanel);

    _spritePreview = std::make_shared<ui::SpritePreview>(_resourceManager);
    float previewElementSize = previewPanelSize - 10.0f;
    _spritePreview->setDisplayBounds(
        math::Vector2f(5.0f, 5.0f),
        math::Vector2f(previewElementSize, previewElementSize)
    );
    _spritePreview->setVisible(true);
    _spritePreviewPanel->addChild(_spritePreview);

    const float labelStartX = 250.0f + previewPanelSize + 15.0f;
    _spriteWidthLabel = std::make_shared<ui::Text>(_resourceManager);
    _spriteWidthLabel->setPosition(math::Vector2f(labelStartX, 165.0f));
    _spriteWidthLabel->setText("Width: 0");
    _spriteWidthLabel->setFontSize(24);
    _spriteWidthLabel->setTextColor(colors::BUTTON_PRIMARY);
    _spriteWidthLabel->setVisible(true);
    _bottomPanel->addChild(_spriteWidthLabel);

    _spriteHeightLabel = std::make_shared<ui::Text>(_resourceManager);
    _spriteHeightLabel->setPosition(math::Vector2f(labelStartX + 150.0f, 165.0f));
    _spriteHeightLabel->setText("Height: 0");
    _spriteHeightLabel->setFontSize(24);
    _spriteHeightLabel->setTextColor(colors::BUTTON_PRIMARY);
    _spriteHeightLabel->setVisible(true);
    _bottomPanel->addChild(_spriteHeightLabel);

    _obstacleTypeLabel = std::make_shared<ui::Text>(_resourceManager);
    _obstacleTypeLabel->setPosition(math::Vector2f(450.0f, 12.5f));
    _obstacleTypeLabel->setText("Type");
    _obstacleTypeLabel->setFontSize(16);
    _obstacleTypeLabel->setTextColor(colors::BUTTON_PRIMARY);
    _obstacleTypeLabel->setVisible(true);
    _bottomPanel->addChild(_obstacleTypeLabel);

    _obstacleTypeDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _obstacleTypeDropdown->setPosition(math::Vector2f(450.0f, 40.0f));
    _obstacleTypeDropdown->setSize(math::Vector2f(200.0f, 35.0f));
    _obstacleTypeDropdown->setScalingEnabled(false);
    _obstacleTypeDropdown->setDirection(ui::DropdownDirection::Right);
    _obstacleTypeDropdown->addOption("unique");
    _obstacleTypeDropdown->addOption("horizontal");
    _obstacleTypeDropdown->addOption("vertical");
    _obstacleTypeDropdown->setSelectedIndex(0);
    _obstacleTypeDropdown->setVisible(true);
    _obstacleTypeDropdown->setOnSelectionChanged(
        [this](const std::string& selectedText, size_t selectedIndex) {
        (void)selectedText;
        if (!_selectedObstacle.has_value() || _isLoadingFromHistory || _isSelectingObject) {
            return;
        }

        const auto& sel = _selectedObstacle.value();
        std::string newType;
        if (selectedIndex == 0) {
            newType = "unique";
        } else if (selectedIndex == 1) {
            newType = "horizontal";
        } else if (selectedIndex == 2) {
            newType = "vertical";
        } else {
            return;
        }

        if (newType == sel.type) {
            return;
        }

        auto& group = _obstaclesByName[sel.prefabName];
        float posX = 0.0f;
        float posY = 0.0f;
        int count = 1;

        if (sel.type == "unique") {
            if (sel.index >= 0 && sel.index < static_cast<int>(group.uniques.size())) {
                const auto& oldObst = group.uniques[static_cast<size_t>(sel.index)];
                posX = oldObst.posX;
                posY = oldObst.posY;
                group.uniques.erase(group.uniques.begin() + sel.index);
            }
        } else if (sel.type == "horizontal") {
            if (sel.index >= 0 && sel.index < static_cast<int>(group.horizontalLines.size())) {
                const auto& oldObst = group.horizontalLines[static_cast<size_t>(sel.index)];
                posX = oldObst.fromX;
                posY = oldObst.posY;
                count = oldObst.count;
                group.horizontalLines.erase(group.horizontalLines.begin() + sel.index);
            }
        } else if (sel.type == "vertical") {
            if (sel.index >= 0 && sel.index < static_cast<int>(group.verticalLines.size())) {
                const auto& oldObst = group.verticalLines[static_cast<size_t>(sel.index)];
                posX = oldObst.posX;
                posY = oldObst.fromY;
                count = oldObst.count;
                group.verticalLines.erase(group.verticalLines.begin() + sel.index);
            }
        }

        int newIndex = 0;
        _isSelectingObject = true;

        if (newType == "unique") {
            UniqueObstacle newObst;
            newObst.posX = posX;
            newObst.posY = posY;
            group.uniques.push_back(newObst);
            newIndex = static_cast<int>(group.uniques.size()) - 1;

            if (_obstacleCountInput) _obstacleCountInput->setVisible(false);
            if (_obstacleCountLabel) _obstacleCountLabel->setVisible(false);
        } else if (newType == "horizontal") {
            HorizontalLineObstacle newObst;
            newObst.fromX = posX;
            newObst.posY = posY;
            newObst.count = count;
            group.horizontalLines.push_back(newObst);
            newIndex = static_cast<int>(group.horizontalLines.size()) - 1;

            if (_obstacleCountInput) {
                _obstacleCountInput->setText(std::to_string(count));
                _obstacleCountInput->setVisible(true);
            }
            if (_obstacleCountLabel) _obstacleCountLabel->setVisible(true);
        } else if (newType == "vertical") {
            VerticalLineObstacle newObst;
            newObst.posX = posX;
            newObst.fromY = posY;
            newObst.count = count;
            group.verticalLines.push_back(newObst);
            newIndex = static_cast<int>(group.verticalLines.size()) - 1;

            if (_obstacleCountInput) {
                _obstacleCountInput->setText(std::to_string(count));
                _obstacleCountInput->setVisible(true);
            }
            if (_obstacleCountLabel) _obstacleCountLabel->setVisible(true);
        }

        ObstacleSelection newSel;
        newSel.prefabName = sel.prefabName;
        newSel.type = newType;
        newSel.index = newIndex;
        _selectedObstacle = newSel;

        if (_obstaclePosXInput) {
            _obstaclePosXInput->setText(std::to_string(static_cast<int>(posX)));
        }
        if (_obstaclePosYInput) {
            _obstaclePosYInput->setText(std::to_string(static_cast<int>(posY)));
        }

        _isSelectingObject = false;
        _hasUnsavedChanges = true;
        updateSaveButtonText();
        _hasPendingChange = true;
        _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME_SHORT;
        _lastChangeTime = 0.0f;
    });
    _bottomPanel->addChild(_obstacleTypeDropdown);

    _obstaclePosXLabel = std::make_shared<ui::Text>(_resourceManager);
    _obstaclePosXLabel->setPosition(math::Vector2f(450.0f, 92.5f));
    _obstaclePosXLabel->setText("Pos X");
    _obstaclePosXLabel->setFontSize(16);
    _obstaclePosXLabel->setTextColor(colors::BUTTON_PRIMARY);
    _obstaclePosXLabel->setVisible(false);
    _bottomPanel->addChild(_obstaclePosXLabel);

    _obstaclePosXInput = std::make_shared<ui::TextInput>(_resourceManager);
    _obstaclePosXInput->setPosition(math::Vector2f(450.0f, 115.0f));
    _obstaclePosXInput->setSize(math::Vector2f(130.0f, 30.0f));
    _obstaclePosXInput->setPlaceholder("X...");
    _obstaclePosXInput->setVisible(false);
    _obstaclePosXInput->setScalingEnabled(false);
    _obstaclePosXInput->setFocusEnabled(true);
    _obstaclePosXInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        std::string filteredText;
        for (char c : text) {
            if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _obstaclePosXInput->setText(filteredText);
            return;
        }

        if (!_selectedObstacle.has_value()) {
            return;
        }
        const auto& selectedSel = _selectedObstacle.value();
        try {
            if (!filteredText.empty()) {
                float newPosX = std::stof(filteredText);
                if (selectedSel.type == "unique") {
                    _obstaclesByName[selectedSel.prefabName].uniques[static_cast<
                        size_t>(selectedSel.index)].posX = newPosX;
                } else if (selectedSel.type == "horizontal") {
                    _obstaclesByName[selectedSel.prefabName].horizontalLines[static_cast<
                        size_t>(selectedSel.index)].fromX = newPosX;
                } else if (selectedSel.type == "vertical") {
                    _obstaclesByName[selectedSel.prefabName].verticalLines[static_cast<
                        size_t>(selectedSel.index)].posX = newPosX;
                }
                _hasUnsavedChanges = true;
                updateSaveButtonText();
                _hasPendingChange = true;
                _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                _lastChangeTime = 0.0f;
            }
        } catch (const std::exception&) {
        }
    });
    _obstaclePosXInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_obstaclePosXInput);
    });
    _bottomPanel->addChild(_obstaclePosXInput);

    _obstaclePosYLabel = std::make_shared<ui::Text>(_resourceManager);
    _obstaclePosYLabel->setPosition(math::Vector2f(595.0f, 92.5f));
    _obstaclePosYLabel->setText("Pos Y");
    _obstaclePosYLabel->setFontSize(16);
    _obstaclePosYLabel->setTextColor(colors::BUTTON_PRIMARY);
    _obstaclePosYLabel->setVisible(false);
    _bottomPanel->addChild(_obstaclePosYLabel);

    _obstaclePosYInput = std::make_shared<ui::TextInput>(_resourceManager);
    _obstaclePosYInput->setPosition(math::Vector2f(595.0f, 115.0f));
    _obstaclePosYInput->setSize(math::Vector2f(130.0f, 30.0f));
    _obstaclePosYInput->setPlaceholder("Y...");
    _obstaclePosYInput->setVisible(false);
    _obstaclePosYInput->setScalingEnabled(false);
    _obstaclePosYInput->setFocusEnabled(true);
    _obstaclePosYInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        std::string filteredText;
        for (char c : text) {
            if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _obstaclePosYInput->setText(filteredText);
            return;
        }

        if (!_selectedObstacle.has_value()) {
            return;
        }
        const auto& selectedSel = _selectedObstacle.value();
        try {
            if (!filteredText.empty()) {
                float newPosY = std::stof(filteredText);
                if (selectedSel.type == "unique") {
                    _obstaclesByName[selectedSel.prefabName].uniques[static_cast<
                        size_t>(selectedSel.index)].posY = newPosY;
                } else if (selectedSel.type == "horizontal") {
                    _obstaclesByName[selectedSel.prefabName].horizontalLines[static_cast<
                        size_t>(selectedSel.index)].posY = newPosY;
                } else if (selectedSel.type == "vertical") {
                    _obstaclesByName[selectedSel.prefabName].verticalLines[static_cast<
                        size_t>(selectedSel.index)].fromY = newPosY;
                }
                _hasUnsavedChanges = true;
                updateSaveButtonText();
                _hasPendingChange = true;
                _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                _lastChangeTime = 0.0f;
            }
        } catch (const std::exception&) {
        }
    });
    _obstaclePosYInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_obstaclePosYInput);
    });
    _bottomPanel->addChild(_obstaclePosYInput);

    _obstacleCountLabel = std::make_shared<ui::Text>(_resourceManager);
    _obstacleCountLabel->setPosition(math::Vector2f(740.0f, 92.5f));
    _obstacleCountLabel->setText("Count");
    _obstacleCountLabel->setFontSize(16);
    _obstacleCountLabel->setTextColor(colors::BUTTON_PRIMARY);
    _obstacleCountLabel->setVisible(false);
    _bottomPanel->addChild(_obstacleCountLabel);

    _obstacleCountInput = std::make_shared<ui::TextInput>(_resourceManager);
    _obstacleCountInput->setPosition(math::Vector2f(740.0f, 115.0f));
    _obstacleCountInput->setSize(math::Vector2f(100.0f, 30.0f));
    _obstacleCountInput->setPlaceholder("Count...");
    _obstacleCountInput->setVisible(false);
    _obstacleCountInput->setScalingEnabled(false);
    _obstacleCountInput->setFocusEnabled(true);
    _obstacleCountInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        std::string filteredText;
        for (char c : text) {
            if ((c >= '0' && c <= '9')) {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _obstacleCountInput->setText(filteredText);
            return;
        }

        if (!_selectedObstacle.has_value()) {
            return;
        }
        const auto& sel = _selectedObstacle.value();
        try {
            if (!filteredText.empty()) {
                int newCount = std::stoi(filteredText);
                if (sel.type == "horizontal") {
                    _obstaclesByName[sel.prefabName].horizontalLines[
                        static_cast<size_t>(sel.index)].count = newCount;
                } else if (sel.type == "vertical") {
                    _obstaclesByName[sel.prefabName].verticalLines[
                        static_cast<size_t>(sel.index)].count = newCount;
                }
                _hasUnsavedChanges = true;
                updateSaveButtonText();
                _hasPendingChange = true;
                _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                _lastChangeTime = 0.0f;
            }
        } catch (const std::exception&) {
        }
    });
    _obstacleCountInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_obstacleCountInput);
    });
    _bottomPanel->addChild(_obstacleCountInput);

    _obstacleDeleteButton = std::make_shared<ui::Button>(_resourceManager);
    _obstacleDeleteButton->setPosition(math::Vector2f(1490, 145.0f));
    _obstacleDeleteButton->setSize(math::Vector2f(110.0f, 40.0f));
    _obstacleDeleteButton->setText("Delete");
    _obstacleDeleteButton->setNormalColor(colors::BUTTON_DANGER);
    _obstacleDeleteButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
    _obstacleDeleteButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);
    _obstacleDeleteButton->setVisible(false);
    _obstacleDeleteButton->setScalingEnabled(false);
    _obstacleDeleteButton->setFocusEnabled(true);
    _obstacleDeleteButton->setOnRelease([this]() {
        if (!_selectedObstacle.has_value()) {
            return;
        }

        const auto& sel = _selectedObstacle.value();

        if (sel.type == "unique") {
            auto& uniques = _obstaclesByName[sel.prefabName].uniques;
            if (sel.index < static_cast<int>(uniques.size())) {
                uniques.erase(uniques.begin() + sel.index);
            }
        } else if (sel.type == "horizontal") {
            auto& horizontals = _obstaclesByName[sel.prefabName].horizontalLines;
            if (sel.index < static_cast<int>(horizontals.size())) {
                horizontals.erase(horizontals.begin() + sel.index);
            }
        } else if (sel.type == "vertical") {
            auto& verticals = _obstaclesByName[sel.prefabName].verticalLines;
            if (sel.index < static_cast<int>(verticals.size())) {
                verticals.erase(verticals.begin() + sel.index);
            }
        }

        _selectedObstacle = std::nullopt;
        hideObstacleUI();
        if (_obstaclePrefabDropdown) {
            _obstaclePrefabDropdown->setSelectedIndex(0);
        }

        _hasUnsavedChanges = true;
        updateSaveButtonText();
        saveToHistory();
    });
    _bottomPanel->addChild(_obstacleDeleteButton);

    _obstacleDuplicateButton = std::make_shared<ui::Button>(_resourceManager);
    _obstacleDuplicateButton->setPosition(math::Vector2f(1490, 92.0f));
    _obstacleDuplicateButton->setSize(math::Vector2f(110.0f, 40.0f));
    _obstacleDuplicateButton->setText("Dup");
    _obstacleDuplicateButton->setNormalColor(colors::BUTTON_SECONDARY);
    _obstacleDuplicateButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _obstacleDuplicateButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _obstacleDuplicateButton->setVisible(false);
    _obstacleDuplicateButton->setScalingEnabled(false);
    _obstacleDuplicateButton->setFocusEnabled(true);
    _obstacleDuplicateButton->setOnRelease([this]() {
        if (!_selectedObstacle.has_value()) {
            return;
        }

        const auto& sel = _selectedObstacle.value();

        if (sel.type == "unique") {
            auto& uniques = _obstaclesByName[sel.prefabName].uniques;
            if (sel.index < static_cast<int>(uniques.size())) {
                auto newObstacle = uniques[static_cast<size_t>(sel.index)];
                newObstacle.posX += 25.0f;
                newObstacle.posY += 25.0f;
                uniques.push_back(newObstacle);

                ObstacleSelection newSel;
                newSel.prefabName = sel.prefabName;
                newSel.type = sel.type;
                newSel.index = static_cast<int>(uniques.size()) - 1;
                _selectedObstacle = newSel;

                if (_obstaclePosXInput) {
                    _obstaclePosXInput->setText(std::to_string(
                        static_cast<int>(newObstacle.posX)));
                }
                if (_obstaclePosYInput) {
                    _obstaclePosYInput->setText(std::to_string(
                        static_cast<int>(newObstacle.posY)));
                }
                if (_obstacleCountInput) {
                    _obstacleCountInput->setVisible(false);
                }
                if (_obstacleCountLabel) {
                    _obstacleCountLabel->setVisible(false);
                }

                _hasUnsavedChanges = true;
                updateSaveButtonText();
            }
        } else if (sel.type == "horizontal") {
            auto& horizontals = _obstaclesByName[sel.prefabName].horizontalLines;
            if (sel.index < static_cast<int>(horizontals.size())) {
                auto newObstacle = horizontals[static_cast<size_t>(sel.index)];
                newObstacle.fromX += 25.0f;
                newObstacle.posY += 25.0f;
                horizontals.push_back(newObstacle);

                ObstacleSelection newSel;
                newSel.prefabName = sel.prefabName;
                newSel.type = sel.type;
                newSel.index = static_cast<int>(horizontals.size()) - 1;
                _selectedObstacle = newSel;

                if (_obstaclePosXInput) {
                    _obstaclePosXInput->setText(std::to_string(
                        static_cast<int>(newObstacle.fromX)));
                }
                if (_obstaclePosYInput) {
                    _obstaclePosYInput->setText(std::to_string(
                        static_cast<int>(newObstacle.posY)));
                }
                if (_obstacleCountInput) {
                    _obstacleCountInput->setText(std::to_string(newObstacle.count));
                }

                _hasUnsavedChanges = true;
                updateSaveButtonText();
            }
        } else if (sel.type == "vertical") {
            auto& verticals = _obstaclesByName[sel.prefabName].verticalLines;
            if (sel.index < static_cast<int>(verticals.size())) {
                auto newObstacle = verticals[static_cast<size_t>(sel.index)];
                newObstacle.posX += 25.0f;
                newObstacle.fromY += 25.0f;
                verticals.push_back(newObstacle);

                ObstacleSelection newSel;
                newSel.prefabName = sel.prefabName;
                newSel.type = sel.type;
                newSel.index = static_cast<int>(verticals.size()) - 1;
                _selectedObstacle = newSel;

                if (_obstaclePosXInput) {
                    _obstaclePosXInput->setText(std::to_string(
                        static_cast<int>(newObstacle.posX)));
                }
                if (_obstaclePosYInput) {
                    _obstaclePosYInput->setText(std::to_string(
                        static_cast<int>(newObstacle.fromY)));
                }
                if (_obstacleCountInput) {
                    _obstacleCountInput->setText(std::to_string(newObstacle.count));
                }

                _hasUnsavedChanges = true;
                updateSaveButtonText();
                saveToHistory();
            }
        }
    });
    _bottomPanel->addChild(_obstacleDuplicateButton);

    /* PowerUp UI elements */
    _powerUpPrefabLabel = std::make_shared<ui::Text>(_resourceManager);
    _powerUpPrefabLabel->setPosition(math::Vector2f(10.0f, 100.0f));
    _powerUpPrefabLabel->setText("Prefab");
    _powerUpPrefabLabel->setFontSize(16);
    _powerUpPrefabLabel->setTextColor(colors::BUTTON_PRIMARY);
    _powerUpPrefabLabel->setVisible(false);
    _bottomPanel->addChild(_powerUpPrefabLabel);

    _powerUpPrefabDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _powerUpPrefabDropdown->setPosition(math::Vector2f(10.0f, 130.0f));
    _powerUpPrefabDropdown->setSize(math::Vector2f(200.0f, 40.0f));
    auto availablePowerUps = loadAvailablePowerUps();
    _powerUpPrefabDropdown->setOptions(availablePowerUps);
    _powerUpPrefabDropdown->setPlaceholder("Prefab...");
    _powerUpPrefabDropdown->setOnSelectionChanged(
        [this](const std::string& powerUp, [[maybe_unused]] size_t index) {
        if (!powerUp.empty()) {
            std::filesystem::path prefabPath =
                std::filesystem::path(constants::POWERUPS_DIRECTORY) / (powerUp +
                    constants::LEVEL_FILE_EXTENSION);
            if (_spritePreview && std::filesystem::exists(prefabPath)) {
                _spritePreview->loadPrefab(prefabPath);

                LevelPreviewSprite spriteData =
                    extractSpriteDataFromPrefab(prefabPath.string());
                std::string widthStr = "Width: " +
                    std::to_string(static_cast<int>(spriteData.width));
                std::string heightStr = "Height: " +
                    std::to_string(static_cast<int>(spriteData.height));
                _spriteWidthLabel->setText(widthStr);
                _spriteHeightLabel->setText(heightStr);

                _powerUpAnimationData[powerUp] = spriteData;
                if (spriteData.isAnimation) {
                    _powerUpAnimationFrames[powerUp] = 0.0f;
                    _powerUpAnimationTimes[powerUp] = 0.0f;
                }
            }
        }
    });
    _powerUpPrefabDropdown->setScalingEnabled(false);
    _powerUpPrefabDropdown->setFocusEnabled(true);
    _powerUpPrefabDropdown->setDirection(ui::DropdownDirection::Right);
    _powerUpPrefabDropdown->setVisible(false);

    _powerUpPosXLabel = std::make_shared<ui::Text>(_resourceManager);
    _powerUpPosXLabel->setPosition(math::Vector2f(450.0f, 92.5f));
    _powerUpPosXLabel->setText("Pos X");
    _powerUpPosXLabel->setFontSize(16);
    _powerUpPosXLabel->setTextColor(colors::BUTTON_PRIMARY);
    _powerUpPosXLabel->setVisible(false);
    _bottomPanel->addChild(_powerUpPosXLabel);

    _powerUpPosXInput = std::make_shared<ui::TextInput>(_resourceManager);
    _powerUpPosXInput->setPosition(math::Vector2f(450.0f, 115.0f));
    _powerUpPosXInput->setSize(math::Vector2f(130.0f, 30.0f));
    _powerUpPosXInput->setPlaceholder("X...");
    _powerUpPosXInput->setVisible(false);
    _powerUpPosXInput->setScalingEnabled(false);
    _powerUpPosXInput->setFocusEnabled(true);
    _powerUpPosXInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        std::string filteredText;
        for (char c : text) {
            if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _powerUpPosXInput->setText(filteredText);
            return;
        }

        if (!_selectedPowerUp.has_value()) {
            return;
        }
        const auto& sel = _selectedPowerUp.value();
        try {
            if (!filteredText.empty()) {
                float newPosX = std::stof(filteredText);
                _powerUpsByName[sel.prefabName][static_cast<
                    size_t>(sel.index)].posX = newPosX;
                _hasUnsavedChanges = true;
                updateSaveButtonText();
                _hasPendingChange = true;
                _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                _lastChangeTime = 0.0f;
            }
        } catch (const std::exception&) {
        }
    });
    _powerUpPosXInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_powerUpPosXInput);
    });
    _bottomPanel->addChild(_powerUpPosXInput);

    _powerUpPosYLabel = std::make_shared<ui::Text>(_resourceManager);
    _powerUpPosYLabel->setPosition(math::Vector2f(595.0f, 92.5f));
    _powerUpPosYLabel->setText("Pos Y");
    _powerUpPosYLabel->setFontSize(16);
    _powerUpPosYLabel->setTextColor(colors::BUTTON_PRIMARY);
    _powerUpPosYLabel->setVisible(false);
    _bottomPanel->addChild(_powerUpPosYLabel);

    _powerUpPosYInput = std::make_shared<ui::TextInput>(_resourceManager);
    _powerUpPosYInput->setPosition(math::Vector2f(595.0f, 115.0f));
    _powerUpPosYInput->setSize(math::Vector2f(130.0f, 30.0f));
    _powerUpPosYInput->setPlaceholder("Y...");
    _powerUpPosYInput->setVisible(false);
    _powerUpPosYInput->setScalingEnabled(false);
    _powerUpPosYInput->setFocusEnabled(true);
    _powerUpPosYInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        std::string filteredText;
        for (char c : text) {
            if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
                filteredText += c;
            }
        }

        if (filteredText != text) {
            _powerUpPosYInput->setText(filteredText);
            return;
        }

        if (!_selectedPowerUp.has_value()) {
            return;
        }
        const auto& sel = _selectedPowerUp.value();
        try {
            if (!filteredText.empty()) {
                float newPosY = std::stof(filteredText);
                _powerUpsByName[sel.prefabName][static_cast<
                    size_t>(sel.index)].posY = newPosY;
                _hasUnsavedChanges = true;
                updateSaveButtonText();
                _hasPendingChange = true;
                _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                _lastChangeTime = 0.0f;
            }
        } catch (const std::exception&) {
        }
    });
    _powerUpPosYInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_powerUpPosYInput);
    });
    _bottomPanel->addChild(_powerUpPosYInput);

    _powerUpDeleteButton = std::make_shared<ui::Button>(_resourceManager);
    _powerUpDeleteButton->setPosition(math::Vector2f(1490, 145.0f));
    _powerUpDeleteButton->setSize(math::Vector2f(110.0f, 40.0f));
    _powerUpDeleteButton->setText("Delete");
    _powerUpDeleteButton->setNormalColor(colors::BUTTON_DANGER);
    _powerUpDeleteButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
    _powerUpDeleteButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);
    _powerUpDeleteButton->setVisible(false);
    _powerUpDeleteButton->setScalingEnabled(false);
    _powerUpDeleteButton->setFocusEnabled(true);
    _powerUpDeleteButton->setOnRelease([this]() {
        if (!_selectedPowerUp.has_value()) {
            return;
        }

        const auto& sel = _selectedPowerUp.value();
        auto& powerUps = _powerUpsByName[sel.prefabName];
        if (sel.index < static_cast<int>(powerUps.size())) {
            powerUps.erase(powerUps.begin() + sel.index);
        }

        _selectedPowerUp = std::nullopt;
        hidePowerUpUI();
        if (_powerUpPrefabDropdown) {
            _powerUpPrefabDropdown->setSelectedIndex(0);
        }

        _hasUnsavedChanges = true;
        updateSaveButtonText();
        saveToHistory();
    });
    _bottomPanel->addChild(_powerUpDeleteButton);

    _powerUpDuplicateButton = std::make_shared<ui::Button>(_resourceManager);
    _powerUpDuplicateButton->setPosition(math::Vector2f(1490, 92.0f));
    _powerUpDuplicateButton->setSize(math::Vector2f(110.0f, 40.0f));
    _powerUpDuplicateButton->setText("Dup");
    _powerUpDuplicateButton->setNormalColor(colors::BUTTON_SECONDARY);
    _powerUpDuplicateButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _powerUpDuplicateButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _powerUpDuplicateButton->setVisible(false);
    _powerUpDuplicateButton->setScalingEnabled(false);
    _powerUpDuplicateButton->setFocusEnabled(true);
    _powerUpDuplicateButton->setOnRelease([this]() {
        if (!_selectedPowerUp.has_value()) {
            return;
        }

        const auto& sel = _selectedPowerUp.value();
        auto& powerUps = _powerUpsByName[sel.prefabName];
        if (sel.index < static_cast<int>(powerUps.size())) {
            auto newPowerUp = powerUps[static_cast<size_t>(sel.index)];
            newPowerUp.posX += 25.0f;
            newPowerUp.posY += 25.0f;
            powerUps.push_back(newPowerUp);

            PowerUpSelection newSel;
            newSel.prefabName = sel.prefabName;
            newSel.index = static_cast<int>(powerUps.size()) - 1;
            _selectedPowerUp = newSel;

            if (_powerUpPosXInput) {
                _powerUpPosXInput->setText(std::to_string(static_cast<int>(newPowerUp.posX)));
            }
            if (_powerUpPosYInput) {
                _powerUpPosYInput->setText(std::to_string(static_cast<int>(newPowerUp.posY)));
            }

            _hasUnsavedChanges = true;
            updateSaveButtonText();
            saveToHistory();
        }
    });
    _bottomPanel->addChild(_powerUpDuplicateButton);

    _waveLabel = std::make_shared<ui::Text>(_resourceManager);
    _waveLabel->setPosition(math::Vector2f(10.0f, 100.0f));
    _waveLabel->setText("Wave:");
    _waveLabel->setFontSize(18);
    _waveLabel->setVisible(false);
    _bottomPanel->addChild(_waveLabel);

    _waveIndexLabel = std::make_shared<ui::Text>(_resourceManager);
    _waveIndexLabel->setPosition(math::Vector2f(80.0f, 100.0f));
    _waveIndexLabel->setText("1 / 1");
    _waveIndexLabel->setFontSize(18);
    _waveIndexLabel->setVisible(false);
    _bottomPanel->addChild(_waveIndexLabel);

    _wavePrevButton = std::make_shared<ui::Button>(_resourceManager);
    _wavePrevButton->setPosition(math::Vector2f(10.0f, 135.0f));
    _wavePrevButton->setSize(math::Vector2f(40.0f, 40.0f));
    _wavePrevButton->setText("-");
    _wavePrevButton->setVisible(false);
    _wavePrevButton->setScalingEnabled(false);
    _wavePrevButton->setFocusEnabled(true);
    _wavePrevButton->setOnRelease([this]() {
        if (!_waves.empty()) {
            _currentWaveIndex = (_currentWaveIndex - 1 + static_cast<int>(
                _waves.size())) % static_cast<int>(_waves.size());
            _selectedWave = WaveSelection{_currentWaveIndex, -1};
            const auto& wave = _waves[static_cast<size_t>(_currentWaveIndex)];

            if (_waveIndexLabel) {
                _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                    " / " + std::to_string(_waves.size()));
            }
            if (_waveTriggerXInput) {
                _waveTriggerXInput->setText(
                    std::to_string(static_cast<int>(wave.gameXTrigger)));
                _waveTriggerXInput->setVisible(true);
            }
            if (_waveTriggerXLabel) {
                _waveTriggerXLabel->setVisible(true);
            }
            if (_waveDeleteButton) {
                _waveDeleteButton->setVisible(true);
            }
        }
    });
    _bottomPanel->addChild(_wavePrevButton);

    _waveNextButton = std::make_shared<ui::Button>(_resourceManager);
    _waveNextButton->setPosition(math::Vector2f(75.0f, 135.0f));
    _waveNextButton->setSize(math::Vector2f(40.0f, 40.0f));
    _waveNextButton->setText("+");
    _waveNextButton->setVisible(false);
    _waveNextButton->setScalingEnabled(false);
    _waveNextButton->setFocusEnabled(true);
    _waveNextButton->setOnRelease([this]() {
        if (!_waves.empty()) {
            _currentWaveIndex = (_currentWaveIndex + 1) % static_cast<int>(_waves.size());
            _selectedWave = WaveSelection{_currentWaveIndex, -1};
            const auto& wave = _waves[static_cast<size_t>(_currentWaveIndex)];

            if (_waveIndexLabel) {
                _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                    " / " + std::to_string(_waves.size()));
            }
            if (_waveTriggerXInput) {
                _waveTriggerXInput->setText(std::to_string(static_cast<int>(
                    wave.gameXTrigger)));
                _waveTriggerXInput->setVisible(true);
            }
            if (_waveTriggerXLabel) {
                _waveTriggerXLabel->setVisible(true);
            }
            if (_waveDeleteButton) {
                _waveDeleteButton->setVisible(true);
            }
        }
    });
    _bottomPanel->addChild(_waveNextButton);

    _waveDeleteButton = std::make_shared<ui::Button>(_resourceManager);
    _waveDeleteButton->setPosition(math::Vector2f(1490.0f, 145.0f));
    _waveDeleteButton->setSize(math::Vector2f(110.0f, 40.0f));
    _waveDeleteButton->setText("Delete");
    _waveDeleteButton->setNormalColor(colors::BUTTON_DANGER);
    _waveDeleteButton->setHoveredColor(colors::BUTTON_DANGER_HOVER);
    _waveDeleteButton->setPressedColor(colors::BUTTON_DANGER_PRESSED);
    _waveDeleteButton->setVisible(false);
    _waveDeleteButton->setScalingEnabled(false);
    _waveDeleteButton->setFocusEnabled(true);
    _waveDeleteButton->setOnRelease([this]() {
        if (_selectedWave.has_value() && _selectedWave.value().waveIndex >= 0 &&
            _selectedWave.value().waveIndex < static_cast<int>(_waves.size())) {
            _waves.erase(_waves.begin() + _selectedWave.value().waveIndex);
            _selectedWave = std::nullopt;

            if (_waveIndexLabel) {
                if (_waves.empty()) {
                    _waveIndexLabel->setText("0 / 0");
                } else {
                    _waveIndexLabel->setText("0 / " + std::to_string(_waves.size()));
                }
            }

            std::string editorMode = "";
            if (_editorModeDropdown) {
                editorMode = _editorModeDropdown->getSelectedOption();
            }
            bool showNavButtons = (editorMode == "Waves" && !_waves.empty());
            if (_wavePrevButton) _wavePrevButton->setVisible(showNavButtons);
            if (_waveNextButton) _waveNextButton->setVisible(showNavButtons);

            hideWaveUI();
            updateEnemyUI();

            _hasUnsavedChanges = true;
            updateSaveButtonText();
            saveToHistory();
        }
    });
    _bottomPanel->addChild(_waveDeleteButton);

    _waveDuplicateButton = std::make_shared<ui::Button>(_resourceManager);
    _waveDuplicateButton->setPosition(math::Vector2f(1490.0f, 92.0f));
    _waveDuplicateButton->setSize(math::Vector2f(110.0f, 40.0f));
    _waveDuplicateButton->setText("Dup");
    _waveDuplicateButton->setNormalColor(colors::BUTTON_SECONDARY);
    _waveDuplicateButton->setHoveredColor(colors::BUTTON_SECONDARY_HOVER);
    _waveDuplicateButton->setPressedColor(colors::BUTTON_SECONDARY_PRESSED);
    _waveDuplicateButton->setVisible(false);
    _waveDuplicateButton->setScalingEnabled(false);
    _waveDuplicateButton->setFocusEnabled(true);
    _waveDuplicateButton->setOnRelease([this]() {
        if (_selectedWave.has_value() && _selectedWave.value().waveIndex >= 0 &&
            _selectedWave.value().waveIndex < static_cast<int>(_waves.size())) {
            auto newWave = _waves[static_cast<size_t>(_selectedWave.value().waveIndex)];
            newWave.gameXTrigger += 25.0f;
            _waves.push_back(newWave);

            WaveSelection newSel;
            newSel.waveIndex = static_cast<int>(_waves.size()) - 1;
            newSel.enemyIndex = -1;
            _selectedWave = newSel;
            _currentWaveIndex = newSel.waveIndex;

            if (_waveIndexLabel) {
                _waveIndexLabel->setText(std::to_string(_currentWaveIndex + 1) +
                    " / " + std::to_string(_waves.size()));
            }

            if (_waveTriggerXInput) {
                _waveTriggerXInput->setText(std::to_string(
                    static_cast<int>(newWave.gameXTrigger)));
            }

            updateEnemyUI();

            _hasUnsavedChanges = true;
            updateSaveButtonText();
            saveToHistory();
        }
    });
    _bottomPanel->addChild(_waveDuplicateButton);

    _waveTriggerXLabel = std::make_shared<ui::Text>(_resourceManager);
    _waveTriggerXLabel->setPosition(math::Vector2f(175.0f, 100.0f));
    _waveTriggerXLabel->setText("Trigger X:");
    _waveTriggerXLabel->setFontSize(18);
    _waveTriggerXLabel->setVisible(false);
    _bottomPanel->addChild(_waveTriggerXLabel);

    _waveTriggerXInput = std::make_shared<ui::TextInput>(_resourceManager);
    _waveTriggerXInput->setPosition(math::Vector2f(175.0f, 135.0f));
    _waveTriggerXInput->setSize(math::Vector2f(120.0f, 40.0f));
    _waveTriggerXInput->setVisible(false);
    _waveTriggerXInput->setScalingEnabled(false);
    _waveTriggerXInput->setFocusEnabled(true);
    _waveTriggerXInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            const auto& sel = _selectedWave.value();
            if (sel.waveIndex >= 0 && sel.waveIndex < static_cast<int>(_waves.size())) {
                try {
                    float triggerX = std::stof(text);
                    _waves[static_cast<size_t>(sel.waveIndex)].gameXTrigger = triggerX;
                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                    _hasPendingChange = true;
                    _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                    _lastChangeTime = 0.0f;
                } catch (const std::exception&) {
                }
            }
        }
    });
    _waveTriggerXInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_waveTriggerXInput);
    });
    _bottomPanel->addChild(_waveTriggerXInput);

    _enemyLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyLabel->setPosition(math::Vector2f(320.0f, 15.0f));
    _enemyLabel->setText("Enemy:");
    _enemyLabel->setFontSize(20);
    _enemyLabel->setVisible(false);
    _bottomPanel->addChild(_enemyLabel);

    _enemyIndexLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyIndexLabel->setPosition(math::Vector2f(320.0f, 45.0f));
    _enemyIndexLabel->setText("0 / 0");
    _enemyIndexLabel->setFontSize(18);
    _enemyIndexLabel->setVisible(false);
    _bottomPanel->addChild(_enemyIndexLabel);

    _enemyPrevButton = std::make_shared<ui::Button>(_resourceManager);
    _enemyPrevButton->setPosition(math::Vector2f(415.0f, 25.0f));
    _enemyPrevButton->setSize(math::Vector2f(40.0f, 45.0f));
    _enemyPrevButton->setText("-");
    _enemyPrevButton->setVisible(false);
    _enemyPrevButton->setOnClick([this]() {
        if (_selectedWave.has_value() && !_waves.empty()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size())) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (!wave.enemies.empty()) {
                    if (enemyIdx < 0) {
                        enemyIdx = static_cast<int>(wave.enemies.size()) - 1;
                    } else {
                        enemyIdx--;
                        if (enemyIdx < 0) {
                            enemyIdx = static_cast<int>(wave.enemies.size()) - 1;
                        }
                    }
                    _currentEnemyIndex = enemyIdx;
                    _selectedWave = WaveSelection{waveIdx, enemyIdx};
                    updateEnemyUI();
                }
            }
        }
    });
    _bottomPanel->addChild(_enemyPrevButton);

    _enemyNextButton = std::make_shared<ui::Button>(_resourceManager);
    _enemyNextButton->setPosition(math::Vector2f(480.0f, 25.0f));
    _enemyNextButton->setSize(math::Vector2f(40.0f, 45.0f));
    _enemyNextButton->setText("+");
    _enemyNextButton->setVisible(false);
    _enemyNextButton->setOnClick([this]() {
        if (_selectedWave.has_value() && !_waves.empty()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size())) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (!wave.enemies.empty()) {
                    if (enemyIdx < 0) {
                        enemyIdx = 0;
                    } else {
                        enemyIdx++;
                        if (enemyIdx >= static_cast<int>(wave.enemies.size())) {
                            enemyIdx = 0;
                        }
                    }
                    _currentEnemyIndex = enemyIdx;
                    _selectedWave = WaveSelection{waveIdx, enemyIdx};
                    updateEnemyUI();
                }
            }
        }
    });
    _bottomPanel->addChild(_enemyNextButton);

    _enemyAddButton = std::make_shared<ui::Button>(_resourceManager);
    _enemyAddButton->setPosition(math::Vector2f(1090.0f, 45.0f));
    _enemyAddButton->setSize(math::Vector2f(200.0f, 40.0f));
    _enemyAddButton->setText("Add Enemy");
    _enemyAddButton->setVisible(false);
    _enemyAddButton->setOnClick([this]() {
        if (_selectedWave.has_value() && !_waves.empty()) {
            int waveIdx = _selectedWave.value().waveIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size())) {
                WaveEnemy newEnemy;
                newEnemy.type = "BasicEnemy";
                newEnemy.count = 1;
                newEnemy.distributionX = {0.0f, 100.0f, "uniform"};
                newEnemy.distributionY = {0.0f, 100.0f, "uniform"};

                _waves[static_cast<size_t>(waveIdx)].enemies.push_back(newEnemy);
                _currentEnemyIndex = static_cast<int>(_waves[static_cast<
                    size_t>(waveIdx)].enemies.size()) - 1;
                _selectedWave = WaveSelection{waveIdx, _currentEnemyIndex};

                updateEnemyUI();
                _hasUnsavedChanges = true;
                updateSaveButtonText();
            }
        }
    });
    _bottomPanel->addChild(_enemyAddButton);

    _enemyDeleteButton = std::make_shared<ui::Button>(_resourceManager);
    _enemyDeleteButton->setPosition(math::Vector2f(1305.0f, 45.0f));
    _enemyDeleteButton->setSize(math::Vector2f(200.0f, 40.0f));
    _enemyDeleteButton->setText("Delete Enemy");
    _enemyDeleteButton->setVisible(false);
    _enemyDeleteButton->setOnClick([this]() {
        if (_selectedWave.has_value() && !_waves.empty()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    wave.enemies.erase(wave.enemies.begin() + enemyIdx);

                    if (wave.enemies.empty()) {
                        _currentEnemyIndex = 0;
                        _selectedWave = WaveSelection{waveIdx, -1};
                    } else {
                        if (_currentEnemyIndex >= static_cast<int>(wave.enemies.size())) {
                            _currentEnemyIndex = static_cast<int>(wave.enemies.size()) - 1;
                        }
                        _selectedWave = WaveSelection{waveIdx, _currentEnemyIndex};
                    }

                    updateEnemyUI();
                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                }
            }
        }
    });
    _bottomPanel->addChild(_enemyDeleteButton);

    _enemyTypeLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyTypeLabel->setPosition(math::Vector2f(700.0f, 15.0f));
    _enemyTypeLabel->setText("Type:");
    _enemyTypeLabel->setFontSize(18);
    _enemyTypeLabel->setVisible(false);
    _bottomPanel->addChild(_enemyTypeLabel);

    _enemyTypeInput = std::make_shared<ui::TextInput>(_resourceManager);
    _enemyTypeInput->setPosition(math::Vector2f(700.0f, 45.0f));
    _enemyTypeInput->setSize(math::Vector2f(250.0f, 40.0f));
    _enemyTypeInput->setVisible(false);
    _enemyTypeInput->setScalingEnabled(false);
    _enemyTypeInput->setFocusEnabled(true);
    _enemyTypeInput->setOnTextChanged([this](const std::string& text) {
        bool isValid = false;
        std::string lowerText = text;
        std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);

        for (const auto& enemy : _availableEnemies) {
            std::string lowerEnemy = enemy;
            std::transform(lowerEnemy.begin(), lowerEnemy.end(),
                lowerEnemy.begin(), ::tolower);
            if (lowerText == lowerEnemy) {
                isValid = true;
                break;
            }
        }

        if (_enemyApplyTypeButton) {
            _enemyApplyTypeButton->setState(
                isValid ? ui::UIState::Normal : ui::UIState::Disabled);
        }
    });
    _enemyTypeInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyTypeInput);
    });
    _bottomPanel->addChild(_enemyTypeInput);

    _enemyApplyTypeButton = std::make_shared<ui::Button>(_resourceManager);
    _enemyApplyTypeButton->setPosition(math::Vector2f(975.0f, 45.0f));
    _enemyApplyTypeButton->setSize(math::Vector2f(100.0f, 40.0f));
    _enemyApplyTypeButton->setText("Apply");
    _enemyApplyTypeButton->setVisible(false);
    _enemyApplyTypeButton->setState(ui::UIState::Disabled);
    _enemyApplyTypeButton->setOnClick([this]() {
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    std::string inputType = _enemyTypeInput->getText();

                    std::string correctType = inputType;
                    std::string lowerInput = inputType;
                    std::transform(lowerInput.begin(), lowerInput.end(),
                        lowerInput.begin(), ::tolower);

                    for (const auto& enemy : _availableEnemies) {
                        std::string lowerEnemy = enemy;
                        std::transform(lowerEnemy.begin(), lowerEnemy.end(),
                            lowerEnemy.begin(), ::tolower);
                        if (lowerInput == lowerEnemy) {
                            correctType = enemy;
                            break;
                        }
                    }

                    wave.enemies[static_cast<size_t>(enemyIdx)].type = correctType;

                    if (_enemyTypeLabel) {
                        _enemyTypeLabel->setText("Type: " + correctType);
                    }

                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                }
            }
        }
    });
    _bottomPanel->addChild(_enemyApplyTypeButton);

    _enemyCountLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyCountLabel->setPosition(math::Vector2f(590.0f, 15.0f));
    _enemyCountLabel->setText("Count:");
    _enemyCountLabel->setFontSize(18);
    _enemyCountLabel->setVisible(false);
    _bottomPanel->addChild(_enemyCountLabel);

    _enemyCountInput = std::make_shared<ui::TextInput>(_resourceManager);
    _enemyCountInput->setPosition(math::Vector2f(590.0f, 45.0f));
    _enemyCountInput->setSize(math::Vector2f(80.0f, 40.0f));
    _enemyCountInput->setVisible(false);
    _enemyCountInput->setScalingEnabled(false);
    _enemyCountInput->setFocusEnabled(true);
    _enemyCountInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    try {
                        int count = std::stoi(text);
                        wave.enemies[static_cast<size_t>(enemyIdx)].count = count;
                        _hasUnsavedChanges = true;
                        updateSaveButtonText();
                        _hasPendingChange = true;
                        _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                        _lastChangeTime = 0.0f;
                    } catch (const std::exception&) {
                    }
                }
            }
        }
    });
    _enemyCountInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyCountInput);
    });
    _bottomPanel->addChild(_enemyCountInput);

    _enemyDistXMinLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyDistXMinLabel->setPosition(math::Vector2f(330.0f, 100.0f));
    _enemyDistXMinLabel->setText("Dist X Min:");
    _enemyDistXMinLabel->setFontSize(18);
    _enemyDistXMinLabel->setVisible(false);
    _bottomPanel->addChild(_enemyDistXMinLabel);

    _enemyDistXMinInput = std::make_shared<ui::TextInput>(_resourceManager);
    _enemyDistXMinInput->setPosition(math::Vector2f(330.0f, 135.0f));
    _enemyDistXMinInput->setSize(math::Vector2f(80.0f, 40.0f));
    _enemyDistXMinInput->setVisible(false);
    _enemyDistXMinInput->setScalingEnabled(false);
    _enemyDistXMinInput->setFocusEnabled(true);
    _enemyDistXMinInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    try {
                        float val = std::stof(text);
                        wave.enemies[static_cast<size_t>(enemyIdx)].distributionX.min = val;
                        _hasUnsavedChanges = true;
                        updateSaveButtonText();
                        _hasPendingChange = true;
                        _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                        _lastChangeTime = 0.0f;
                    } catch (const std::exception&) {
                    }
                }
            }
        }
    });
    _enemyDistXMinInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyDistXMinInput);
    });
    _bottomPanel->addChild(_enemyDistXMinInput);

    _enemyDistXMaxLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyDistXMaxLabel->setPosition(math::Vector2f(470.0f, 100.0f));
    _enemyDistXMaxLabel->setText("Dist X Max:");
    _enemyDistXMaxLabel->setFontSize(18);
    _enemyDistXMaxLabel->setVisible(false);
    _bottomPanel->addChild(_enemyDistXMaxLabel);

    _enemyDistXMaxInput = std::make_shared<ui::TextInput>(_resourceManager);
    _enemyDistXMaxInput->setPosition(math::Vector2f(470.0f, 135.0f));
    _enemyDistXMaxInput->setSize(math::Vector2f(80.0f, 40.0f));
    _enemyDistXMaxInput->setVisible(false);
    _enemyDistXMaxInput->setScalingEnabled(false);
    _enemyDistXMaxInput->setFocusEnabled(true);
    _enemyDistXMaxInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    try {
                        float val = std::stof(text);
                        wave.enemies[static_cast<size_t>(enemyIdx)].distributionX.max = val;
                        _hasUnsavedChanges = true;
                        updateSaveButtonText();
                        _hasPendingChange = true;
                        _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                        _lastChangeTime = 0.0f;
                    } catch (const std::exception&) {
                    }
                }
            }
        }
    });
    _enemyDistXMaxInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyDistXMaxInput);
    });
    _bottomPanel->addChild(_enemyDistXMaxInput);

    _enemyDistYMinLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyDistYMinLabel->setPosition(math::Vector2f(610.0f, 100.0f));
    _enemyDistYMinLabel->setText("Dist Y Min:");
    _enemyDistYMinLabel->setFontSize(18);
    _enemyDistYMinLabel->setVisible(false);
    _bottomPanel->addChild(_enemyDistYMinLabel);

    _enemyDistYMinInput = std::make_shared<ui::TextInput>(_resourceManager);
    _enemyDistYMinInput->setPosition(math::Vector2f(610.0f, 135.0f));
    _enemyDistYMinInput->setSize(math::Vector2f(80.0f, 40.0f));
    _enemyDistYMinInput->setVisible(false);
    _enemyDistYMinInput->setScalingEnabled(false);
    _enemyDistYMinInput->setFocusEnabled(true);
    _enemyDistYMinInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    try {
                        float val = std::stof(text);
                        wave.enemies[static_cast<size_t>(enemyIdx)].distributionY.min = val;
                        _hasUnsavedChanges = true;
                        updateSaveButtonText();
                        _hasPendingChange = true;
                        _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                        _lastChangeTime = 0.0f;
                    } catch (const std::exception&) {
                    }
                }
            }
        }
    });
    _enemyDistYMinInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyDistYMinInput);
    });
    _bottomPanel->addChild(_enemyDistYMinInput);

    _enemyDistYMaxLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyDistYMaxLabel->setPosition(math::Vector2f(750.0f, 100.0f));
    _enemyDistYMaxLabel->setText("Dist Y Max:");
    _enemyDistYMaxLabel->setFontSize(18);
    _enemyDistYMaxLabel->setVisible(false);
    _bottomPanel->addChild(_enemyDistYMaxLabel);

    _enemyDistYMaxInput = std::make_shared<ui::TextInput>(_resourceManager);
    _enemyDistYMaxInput->setPosition(math::Vector2f(750.0f, 135.0f));
    _enemyDistYMaxInput->setSize(math::Vector2f(80.0f, 40.0f));
    _enemyDistYMaxInput->setVisible(false);
    _enemyDistYMaxInput->setScalingEnabled(false);
    _enemyDistYMaxInput->setFocusEnabled(true);
    _enemyDistYMaxInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    try {
                        float val = std::stof(text);
                        wave.enemies[static_cast<size_t>(enemyIdx)].distributionY.max = val;
                        _hasUnsavedChanges = true;
                        updateSaveButtonText();
                        _hasPendingChange = true;
                        _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME;
                        _lastChangeTime = 0.0f;
                    } catch (const std::exception&) {
                    }
                }
            }
        }
    });
    _enemyDistYMaxInput->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyDistYMaxInput);
    });
    _bottomPanel->addChild(_enemyDistYMaxInput);

    _enemyDistXTypeLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyDistXTypeLabel->setPosition(math::Vector2f(890.0f, 100.0f));
    _enemyDistXTypeLabel->setText("Dist X Type:");
    _enemyDistXTypeLabel->setFontSize(18);
    _enemyDistXTypeLabel->setVisible(false);
    _bottomPanel->addChild(_enemyDistXTypeLabel);

    _enemyDistXTypeDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _enemyDistXTypeDropdown->setPosition(math::Vector2f(890.0f, 135.0f));
    _enemyDistXTypeDropdown->setSize(math::Vector2f(150.0f, 40.0f));
    _enemyDistXTypeDropdown->setOptions({"uniform", "random"});
    _enemyDistXTypeDropdown->setVisible(false);
    _enemyDistXTypeDropdown->setDirection(ui::DropdownDirection::Right);
    _enemyDistXTypeDropdown->setScalingEnabled(false);
    _enemyDistXTypeDropdown->setFocusEnabled(true);
    _enemyDistXTypeDropdown->setOnSelectionChanged(
        [this](const std::string& type, [[maybe_unused]] size_t index) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    wave.enemies[static_cast<size_t>(enemyIdx)].distributionX.type = type;
                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                    _hasPendingChange = true;
                    _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME_SHORT;
                    _lastChangeTime = 0.0f;
                }
            }
        }
    });
    _enemyDistXTypeDropdown->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyDistXTypeDropdown);
    });

    _enemyDistYTypeLabel = std::make_shared<ui::Text>(_resourceManager);
    _enemyDistYTypeLabel->setPosition(math::Vector2f(1030.0f, 100.0f));
    _enemyDistYTypeLabel->setText("Dist Y Type:");
    _enemyDistYTypeLabel->setFontSize(18);
    _enemyDistYTypeLabel->setVisible(false);
    _bottomPanel->addChild(_enemyDistYTypeLabel);

    _enemyDistYTypeDropdown = std::make_shared<ui::Dropdown>(_resourceManager);
    _enemyDistYTypeDropdown->setPosition(math::Vector2f(1070.0f, 135.0f));
    _enemyDistYTypeDropdown->setSize(math::Vector2f(150.0f, 40.0f));
    _enemyDistYTypeDropdown->setOptions({"uniform", "random"});
    _enemyDistYTypeDropdown->setVisible(false);
    _enemyDistYTypeDropdown->setDirection(ui::DropdownDirection::Right);
    _enemyDistYTypeDropdown->setScalingEnabled(false);
    _enemyDistYTypeDropdown->setFocusEnabled(true);
    _enemyDistYTypeDropdown->setOnSelectionChanged(
        [this](const std::string& type, [[maybe_unused]] size_t index) {
        if (_isLoadingFromHistory || _isSelectingObject) {
            return;
        }
        if (_selectedWave.has_value()) {
            int waveIdx = _selectedWave.value().waveIndex;
            int enemyIdx = _selectedWave.value().enemyIndex;
            if (waveIdx >= 0 && waveIdx < static_cast<int>(_waves.size()) &&
                enemyIdx >= 0) {
                auto& wave = _waves[static_cast<size_t>(waveIdx)];
                if (enemyIdx < static_cast<int>(wave.enemies.size())) {
                    wave.enemies[static_cast<size_t>(enemyIdx)].distributionY.type = type;
                    _hasUnsavedChanges = true;
                    updateSaveButtonText();
                    _hasPendingChange = true;
                    _currentDebounceTime = constants::CHANGE_DEBOUNCE_TIME_SHORT;
                    _lastChangeTime = 0.0f;
                }
            }
        }
    });
    _enemyDistYTypeDropdown->setOnRelease([this]() {
        auto navMan = this->_uiManager->getNavigationManager();
        navMan->enableFocus();
        navMan->setFocus(this->_enemyDistYTypeDropdown);
    });
    _bottomPanel->addChild(_enemyDistYTypeDropdown);
    _bottomPanel->addChild(_enemyDistXTypeDropdown);

    /* DropDowns are added at the end */
    _bottomPanel->addChild(_obstaclePrefabDropdown);
    _bottomPanel->addChild(_powerUpPrefabDropdown);
    _bottomPanel->addChild(_editorModeDropdown);
}

void LevelEditorState::updateSaveButtonText() {
    if (_saveButton) {
        std::string baseText = "Save Level";
        if (_hasUnsavedChanges) {
            baseText += "*";
        }
        _saveButton->setText(baseText);
    }
}

bool LevelEditorState::validateFields() {
    std::string levelName = _levelNameInput->getText();
    if (levelName.empty()) {
        return false;
    }

    std::string mapLengthStr = _mapLengthInput->getText();
    try {
        float mapLength = std::stof(mapLengthStr);
        if (mapLength <= 0.0f) {
            return false;
        }
    } catch (const std::exception&) {
        return false;
    }

    std::string scrollSpeedStr = _scrollSpeedInput->getText();
    try {
        int scrollSpeed = std::stoi(scrollSpeedStr);
        if (scrollSpeed <= 0) {
            return false;
        }
    } catch (const std::exception&) {
        return false;
    }

    std::string selectedMusic = _musicDropdown->getSelectedOption();
    if (selectedMusic.empty()) {
        return false;
    }

    std::string selectedBackground = _backgroundDropdown->getSelectedOption();
    if (selectedBackground.empty()) {
        return false;
    }

    return true;
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

void LevelEditorState::saveToHistory() {
    saveObstacles();
    savePowerUps();
    saveWaves();

    if (_currentHistoryIndex + 1 < _history.size()) {
        _history.erase(_history.begin() + static_cast<
            std::vector<nlohmann::json>::difference_type>(
                _currentHistoryIndex) + 1, _history.end());
    }

    _history.push_back(_levelData);
    _currentHistoryIndex = _history.size() - 1;

    if (_history.size() > constants::MAX_HISTORY_SIZE) {
        _history.erase(_history.begin());
        _currentHistoryIndex--;
    }

    updateHistoryButtons();
}

void LevelEditorState::loadFromHistory(size_t index) {
    if (index >= _history.size()) {
        return;
    }

    _isLoadingFromHistory = true;
    _levelData = _history[index];
    _currentHistoryIndex = index;

    _levelNameInput->setText(_levelData.value(constants::LEVEL_NAME_FIELD, "New Level"));
    _mapLengthInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0))));
    _scrollSpeedInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::LEVEL_SCROLL_SPEED_FIELD, 1))));

    std::string currentMusic = _levelData.value(constants::LEVEL_MUSIC_FIELD, "");
    if (!currentMusic.empty()) {
        auto options = _musicDropdown->getOptions();
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == currentMusic) {
                _musicDropdown->setSelectedIndex(i);
                break;
            }
        }
    } else {
        _musicDropdown->setSelectedIndex(static_cast<size_t>(-1));
    }

    std::string currentBackground = _levelData.value(constants::LEVEL_BACKGROUND_FIELD, "");
    if (!currentBackground.empty()) {
        auto options = _backgroundDropdown->getOptions();
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i] == currentBackground) {
                _backgroundDropdown->setSelectedIndex(i);
                break;
            }
        }
    } else {
        _backgroundDropdown->setSelectedIndex(static_cast<size_t>(-1));
    }

    _hasUnsavedChanges = true;
    updateSaveButtonText();
    updateHistoryButtons();

    parseObstacles();
    parsePowerUps();
    parseWaves();

    _selectedObstacle = std::nullopt;
    _selectedPowerUp = std::nullopt;
    _selectedWave = std::nullopt;

    hideAllUIElements();

    if (_waveIndexLabel) {
        if (_waves.empty()) {
            _waveIndexLabel->setText("0 / 0");
        } else {
            _waveIndexLabel->setText("0 / " + std::to_string(_waves.size()));
        }
        _waveIndexLabel->setVisible(false);
    }

    _isLoadingFromHistory = false;
    _hasPendingChange = false;
    _lastChangeTime = 0.0f;
}

void LevelEditorState::updateHistoryButtons() {
    bool canUndo = _currentHistoryIndex > 0;
    bool canRedo = _currentHistoryIndex < _history.size() - 1;

    if (_undoButton) {
        _undoButton->setState(canUndo ? ui::UIState::Normal : ui::UIState::Disabled);
    }
    if (_redoButton) {
        _redoButton->setState(canRedo ? ui::UIState::Normal : ui::UIState::Disabled);
    }
}

void LevelEditorState::initializeViewport() {
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;
    _viewportZoom = canvasHeight / constants::MAX_HEIGHT;
    _viewportOffset = math::Vector2f(0.0f, 0.0f);
}

void LevelEditorState::handleZoom(
    [[maybe_unused]] float deltaTime,
    gfx::EventType eventResult
) {
    auto event = _resourceManager->get<gfx::IEvent>();

    bool zoomIn = event->isKeyPressed(gfx::EventType::NUMPAD_ADD);
    bool zoomOut = event->isKeyPressed(gfx::EventType::NUMPAD_SUBTRACT);

    bool wheelUp = eventResult == gfx::EventType::MOUSEWHEELUP;
    bool wheelDown = eventResult == gfx::EventType::MOUSEWHEELDOWN;

    if (zoomIn || zoomOut || wheelUp || wheelDown) {
        const float sidePanelWidth = 300.0f;
        const float bottomPanelHeight = 200.0f;
        const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

        math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();

        bool isInCanvas = mousePos.getX() >= sidePanelWidth &&
                          mousePos.getX() <= constants::MAX_WIDTH &&
                          mousePos.getY() >= 0.0f &&
                          mousePos.getY() <= canvasHeight;

        float cursorLevelX =
            _viewportOffset.getX() + (mousePos.getX() - sidePanelWidth) / _viewportZoom;
        float cursorLevelY = _viewportOffset.getY() + mousePos.getY() / _viewportZoom;

        float zoomFactor = (zoomIn || wheelUp) ? 1.1f : 0.9f;
        float oldZoom = _viewportZoom;
        float newZoom = _viewportZoom * zoomFactor;

        if (newZoom > _maxZoom)
            newZoom = _maxZoom;

        if (newZoom < _minZoom)
            newZoom = _minZoom;

        _viewportZoom = newZoom;

        if (isInCanvas && oldZoom != 0.0f) {
            float newOffsetX = cursorLevelX - (mousePos.getX() - sidePanelWidth) / newZoom;
            float newOffsetY = cursorLevelY - mousePos.getY() / newZoom;
            _viewportOffset.setX(newOffsetX);
            _viewportOffset.setY(newOffsetY);
        }
    }
}

void LevelEditorState::handleCanvasDrag([[maybe_unused]] float deltaTime) {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool rightMousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::RIGHT));

    bool isInCanvas = mousePos.getX() >= sidePanelWidth &&
                      mousePos.getX() <= constants::MAX_WIDTH &&
                      mousePos.getY() >= 0.0f &&
                      mousePos.getY() <= canvasHeight;

    if (rightMousePressed && isInCanvas) {
        if (!_isDragging) {
            _isDragging = true;
            _dragStartPos = mousePos;
            _lastMousePos = mousePos;
        } else {
            math::Vector2f mouseDelta = mousePos - _lastMousePos;

            _viewportOffset.setX(_viewportOffset.getX() - mouseDelta.getX() / _viewportZoom);
            _viewportOffset.setY(_viewportOffset.getY() - mouseDelta.getY() / _viewportZoom);
            _lastMousePos = mousePos;
        }
    } else {
        _isDragging = false;
    }
}

void LevelEditorState::renderLevelPreview() {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    float mapLength = _levelData.value(constants::LEVEL_MAP_LENGTH_FIELD, 0.0f);

    if (mapLength <= 0.0f) {
        return;
    }

    float levelWidth = mapLength * _viewportZoom;
    float levelHeight = constants::MAX_HEIGHT * _viewportZoom;
    float levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
    float levelY = -(_viewportOffset.getY() * _viewportZoom);

    const float canvasLeft = sidePanelWidth;
    const float canvasRight = constants::MAX_WIDTH;
    const float canvasTop = 0.0f;
    const float canvasBottom = canvasHeight;

    if (levelX + levelWidth < canvasLeft || levelX > canvasRight ||
        levelY + levelHeight < canvasTop || levelY > canvasBottom) {
        return;
    }

    auto window = _resourceManager->get<gfx::IWindow>();
    gfx::color_t red = colors::RED;

    const float borderThickness = 2.0f;

    if (levelY >= canvasTop && levelY < canvasBottom) {
        float topX = (std::max)(levelX, canvasLeft);
        float topXEnd = (std::min)(levelX + levelWidth, canvasRight);
        float topWidth = topXEnd - topX;
        if (topWidth > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(topX), static_cast<size_t>(levelY)),
                std::make_pair(static_cast<size_t>(topWidth),
                    static_cast<size_t>(borderThickness))
            );
        }
    }

    float bottomY = levelY + levelHeight - borderThickness;
    if (bottomY >= canvasTop && bottomY < canvasBottom) {
        float bottomX = (std::max)(levelX, canvasLeft);
        float bottomXEnd = (std::min)(levelX + levelWidth, canvasRight);
        float bottomWidth = bottomXEnd - bottomX;
        if (bottomWidth > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(bottomX), static_cast<size_t>(bottomY)),
                std::make_pair(static_cast<size_t>(bottomWidth),
                    static_cast<size_t>(borderThickness))
            );
        }
    }

    if (levelX >= canvasLeft && levelX < canvasRight) {
        float leftY = (std::max)(levelY, canvasTop);
        float leftYEnd = (std::min)(levelY + levelHeight, canvasBottom);
        float leftHeight = leftYEnd - leftY;
        if (leftHeight > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(levelX), static_cast<size_t>(leftY)),
                std::make_pair(static_cast<size_t>(borderThickness),
                    static_cast<size_t>(leftHeight))
            );
        }
    }

    float rightX = levelX + levelWidth - borderThickness;
    if (rightX >= canvasLeft && rightX < canvasRight) {
        float rightY = (std::max)(levelY, canvasTop);
        float rightYEnd = (std::min)(levelY + levelHeight, canvasBottom);
        float rightHeight = rightYEnd - rightY;
        if (rightHeight > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(rightX), static_cast<size_t>(rightY)),
                std::make_pair(static_cast<size_t>(borderThickness),
                    static_cast<size_t>(rightHeight))
            );
        }
    }

    if (_displayFilter == "All" || _displayFilter == "Obstacles") {
        renderAllObstacles(levelX, levelY, canvasLeft, canvasRight, canvasTop, canvasBottom);
    }
    if (_displayFilter == "All" || _displayFilter == "PowerUps") {
        renderAllPowerUps(levelX, levelY, canvasLeft, canvasRight, canvasTop, canvasBottom);
    }
    if (_displayFilter == "All" || _displayFilter == "Waves") {
        renderAllWaves(levelX, levelY, canvasLeft, canvasRight, canvasTop, canvasBottom);
    }
}

void LevelEditorState::renderSpriteInLevelPreview(
    const LevelPreviewSprite& spriteData,
    const std::string& prefabName,
    float screenX,
    float screenY,
    float canvasLeft,
    float canvasRight,
    float canvasTop,
    float canvasBottom
) {
    float scaledWidth = spriteData.width * _viewportZoom;
    float scaledHeight = spriteData.height * _viewportZoom;

    if (screenX + scaledWidth <= canvasLeft || screenX >= canvasRight ||
        screenY + scaledHeight <= canvasTop || screenY >= canvasBottom) {
        return;
    }

    if (screenX >= canvasLeft && screenX + scaledWidth <= canvasRight &&
        screenY >= canvasTop && screenY + scaledHeight <= canvasBottom) {
        auto window = _resourceManager->get<gfx::IWindow>();
        float finalScaleX = _viewportZoom * spriteData.scale;
        float finalScaleY = _viewportZoom * spriteData.scale;

        if (spriteData.isAnimation && spriteData.frameCount > 0.0f) {
            float currentFrameFloat = 0.0f;
            if (_obstacleAnimationFrames.find(prefabName) != _obstacleAnimationFrames.end()) {
                currentFrameFloat = _obstacleAnimationFrames[prefabName];
            } else if (_powerUpAnimationFrames.find(prefabName) !=
                _powerUpAnimationFrames.end()) {
                currentFrameFloat = _powerUpAnimationFrames[prefabName];
            }
            int currentFrameIndex = static_cast<int>(currentFrameFloat);
            float frameX = currentFrameIndex * spriteData.frameWidth;
            float frameY = 0.0f;
            math::FRect frameRect(
                frameX, frameY, spriteData.frameWidth, spriteData.frameHeight);

            window->drawSprite(
                spriteData.texturePath,
                screenX,
                screenY,
                frameRect,
                finalScaleX,
                finalScaleY,
                spriteData.rotation
            );
        } else {
            window->drawSprite(
                spriteData.texturePath,
                screenX,
                screenY,
                finalScaleX,
                finalScaleY,
                spriteData.rotation
            );
        }

        if (_showHitboxes) {
            gfx::color_t green = colors::GREEN;
            window->drawRectangleOutline(
                green,
                std::make_pair(static_cast<size_t>
                    (screenX), static_cast<size_t>(screenY)),
                std::make_pair(static_cast<size_t>
                    (scaledWidth), static_cast<size_t>(scaledHeight)),
                1
            );
        }
    }
}

LevelPreviewSprite LevelEditorState::extractSpriteDataFromPrefab(
    const std::string& prefabPath
) {
    LevelPreviewSprite result;
    result.texturePath = "";
    result.width = 0.0f;
    result.height = 0.0f;
    result.scale = 1.0f;
    result.rotation = 0.0f;

    try {
        std::ifstream file(prefabPath);
        if (!file.is_open()) {
            return result;
        }

        nlohmann::json prefabData;
        file >> prefabData;
        file.close();

        if (!prefabData.contains(constants::PREFAB_COMPONENTS_FIELD)) {
            return result;
        }

        const auto& components = prefabData[constants::PREFAB_COMPONENTS_FIELD];

        if (components.contains(constants::PREFAB_ANIMATION_COMPONENT)) {
            const auto& animComponent = components[constants::PREFAB_ANIMATION_COMPONENT];
            if (animComponent.contains(constants::PREFAB_TEXTURE_PATH_FIELD)) {
                result.texturePath =
                    animComponent[constants::PREFAB_TEXTURE_PATH_FIELD].get<std::string>();
            }

            result.isAnimation = true;
            if (animComponent.contains(constants::PREFAB_STATES_FIELD)) {
                const auto& states = animComponent[constants::PREFAB_STATES_FIELD];
                if (!states.empty()) {
                    auto it = states.begin();
                    const auto& firstState = it.value();

                    if (result.texturePath.empty() &&
                        firstState.contains(constants::PREFAB_TEXTURE_PATH_FIELD)) {
                        result.texturePath = firstState[
                            constants::PREFAB_TEXTURE_PATH_FIELD].get<std::string>();
                    }

                    result.frameCount = firstState.value(
                        constants::PREFAB_FRAME_COUNT_FIELD, 1.0f);
                    result.frameWidth = firstState.value(
                        constants::PREFAB_FRAME_WIDTH_FIELD, 0.0f);
                    result.frameHeight = firstState.value(
                        constants::PREFAB_FRAME_HEIGHT_FIELD, 0.0f);
                    result.animationSpeed = firstState.value(
                        constants::PREFAB_SPEED_FIELD, 0.1f);
                    result.animationLoop = firstState.value(
                        constants::PREFAB_LOOP_FIELD, true);
                }
            }
        } else if (components.contains(constants::PREFAB_SPRITE_COMPONENT)) {
            const auto& spriteComponent = components[constants::PREFAB_SPRITE_COMPONENT];
            if (spriteComponent.contains(constants::PREFAB_FILEPATH_FIELD)) {
                result.texturePath =
                    spriteComponent[constants::PREFAB_FILEPATH_FIELD].get<std::string>();
            }
        }

        if (components.contains(constants::PREFAB_COLLIDER_COMPONENT)) {
            const auto& colliderComponent = components[constants::PREFAB_COLLIDER_COMPONENT];
            if (colliderComponent.contains(constants::PREFAB_SIZE_FIELD)) {
                const auto& size = colliderComponent[constants::PREFAB_SIZE_FIELD];
                if (size.contains(constants::PREFAB_X_FIELD)) {
                    result.width = size[constants::PREFAB_X_FIELD].get<float>();
                }
                if (size.contains(constants::PREFAB_Y_FIELD)) {
                    result.height = size[constants::PREFAB_Y_FIELD].get<float>();
                }
            }
        }

        if (components.contains(constants::PREFAB_TRANSFORM_COMPONENT)) {
            const auto& transformComponent =
                components[constants::PREFAB_TRANSFORM_COMPONENT];
            if (transformComponent.contains(constants::PREFAB_SCALE_FIELD)) {
                const auto& scale = transformComponent[constants::PREFAB_SCALE_FIELD];
                if (scale.contains(constants::PREFAB_X_FIELD)) {
                    result.scale = scale[constants::PREFAB_X_FIELD].get<float>();
                }
            }
            if (transformComponent.contains(constants::PREFAB_ROTATION_FIELD)) {
                result.rotation =
                    transformComponent[constants::PREFAB_ROTATION_FIELD].get<float>();
            }
        }

        result.width *= result.scale;
        result.height *= result.scale;
    } catch (const std::exception&) {
    }

    return result;
}

void LevelEditorState::hideAllUIElements() {
    hideObstacleUI();
    hidePowerUpUI();
    hideWaveUI();
    hideEnemyUI();
}

void LevelEditorState::hideObstacleUI() {
    if (_obstaclePosXInput) _obstaclePosXInput->setVisible(false);
    if (_obstaclePosXLabel) _obstaclePosXLabel->setVisible(false);
    if (_obstaclePosYInput) _obstaclePosYInput->setVisible(false);
    if (_obstaclePosYLabel) _obstaclePosYLabel->setVisible(false);
    if (_obstacleCountInput) _obstacleCountInput->setVisible(false);
    if (_obstacleCountLabel) _obstacleCountLabel->setVisible(false);
    if (_obstacleDeleteButton) _obstacleDeleteButton->setVisible(false);
    if (_obstacleDuplicateButton) _obstacleDuplicateButton->setVisible(false);
}

void LevelEditorState::hidePowerUpUI() {
    if (_powerUpPosXInput) _powerUpPosXInput->setVisible(false);
    if (_powerUpPosXLabel) _powerUpPosXLabel->setVisible(false);
    if (_powerUpPosYInput) _powerUpPosYInput->setVisible(false);
    if (_powerUpPosYLabel) _powerUpPosYLabel->setVisible(false);
    if (_powerUpDeleteButton) _powerUpDeleteButton->setVisible(false);
    if (_powerUpDuplicateButton) _powerUpDuplicateButton->setVisible(false);
}

void LevelEditorState::hideWaveUI() {
    if (_waveTriggerXInput) _waveTriggerXInput->setVisible(false);
    if (_waveTriggerXLabel) _waveTriggerXLabel->setVisible(false);
    if (_waveDeleteButton) _waveDeleteButton->setVisible(false);
    if (_waveDuplicateButton) _waveDuplicateButton->setVisible(false);
    if (_waveIndexLabel) _waveIndexLabel->setVisible(false);
    if (_wavePrevButton) _wavePrevButton->setVisible(false);
    if (_waveNextButton) _waveNextButton->setVisible(false);
}

void LevelEditorState::hideEnemyUI() {
    if (_enemyLabel) _enemyLabel->setVisible(false);
    if (_enemyIndexLabel) _enemyIndexLabel->setVisible(false);
    if (_enemyPrevButton) _enemyPrevButton->setVisible(false);
    if (_enemyNextButton) _enemyNextButton->setVisible(false);
    if (_enemyAddButton) _enemyAddButton->setVisible(false);
    if (_enemyDeleteButton) _enemyDeleteButton->setVisible(false);
    if (_enemyTypeLabel) _enemyTypeLabel->setVisible(false);
    if (_enemyTypeInput) _enemyTypeInput->setVisible(false);
    if (_enemyApplyTypeButton) _enemyApplyTypeButton->setVisible(false);
    if (_enemyAppliedTypeLabel) _enemyAppliedTypeLabel->setVisible(false);
    if (_enemyCountLabel) _enemyCountLabel->setVisible(false);
    if (_enemyCountInput) _enemyCountInput->setVisible(false);
    if (_enemyDistXMinLabel) _enemyDistXMinLabel->setVisible(false);
    if (_enemyDistXMinInput) _enemyDistXMinInput->setVisible(false);
    if (_enemyDistXMaxLabel) _enemyDistXMaxLabel->setVisible(false);
    if (_enemyDistXMaxInput) _enemyDistXMaxInput->setVisible(false);
    if (_enemyDistYMinLabel) _enemyDistYMinLabel->setVisible(false);
    if (_enemyDistYMinInput) _enemyDistYMinInput->setVisible(false);
    if (_enemyDistYMaxLabel) _enemyDistYMaxLabel->setVisible(false);
    if (_enemyDistYMaxInput) _enemyDistYMaxInput->setVisible(false);
    if (_enemyDistXTypeLabel) _enemyDistXTypeLabel->setVisible(false);
    if (_enemyDistXTypeDropdown) _enemyDistXTypeDropdown->setVisible(false);
    if (_enemyDistYTypeLabel) _enemyDistYTypeLabel->setVisible(false);
    if (_enemyDistYTypeDropdown) _enemyDistYTypeDropdown->setVisible(false);
}

void LevelEditorState::showObstacleUI(bool showCount) {
    if (_obstaclePosXInput) _obstaclePosXInput->setVisible(true);
    if (_obstaclePosXLabel) _obstaclePosXLabel->setVisible(true);
    if (_obstaclePosYInput) _obstaclePosYInput->setVisible(true);
    if (_obstaclePosYLabel) _obstaclePosYLabel->setVisible(true);
    if (_obstacleCountInput) _obstacleCountInput->setVisible(showCount);
    if (_obstacleCountLabel) _obstacleCountLabel->setVisible(showCount);
    if (_obstacleDeleteButton) _obstacleDeleteButton->setVisible(true);
    if (_obstacleDuplicateButton) _obstacleDuplicateButton->setVisible(true);
}

void LevelEditorState::showPowerUpUI() {
    if (_powerUpPosXInput) _powerUpPosXInput->setVisible(true);
    if (_powerUpPosXLabel) _powerUpPosXLabel->setVisible(true);
    if (_powerUpPosYInput) _powerUpPosYInput->setVisible(true);
    if (_powerUpPosYLabel) _powerUpPosYLabel->setVisible(true);
    if (_powerUpDeleteButton) _powerUpDeleteButton->setVisible(true);
    if (_powerUpDuplicateButton) _powerUpDuplicateButton->setVisible(true);
}

void LevelEditorState::showWaveUI() {
    if (_waveTriggerXInput) _waveTriggerXInput->setVisible(true);
    if (_waveTriggerXLabel) _waveTriggerXLabel->setVisible(true);
    if (_waveDeleteButton) _waveDeleteButton->setVisible(true);
    if (_waveDuplicateButton) _waveDuplicateButton->setVisible(true);
}

LevelEditorState::WorldCoordinates LevelEditorState::extractWorldCoordinates(
    const math::Vector2f& mousePos, float sidePanelWidth) const {
    WorldCoordinates coords;
    coords.levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
    coords.levelY = -(_viewportOffset.getY() * _viewportZoom);
    coords.worldX = (mousePos.getX() - coords.levelX) / _viewportZoom;
    coords.worldY = (mousePos.getY() - coords.levelY) / _viewportZoom;
    return coords;
}

}  // namespace gsm
