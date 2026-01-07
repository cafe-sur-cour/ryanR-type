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

    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    bool isInCanvas = mousePos.getX() >= sidePanelWidth &&
                      mousePos.getX() <= constants::MAX_WIDTH &&
                      mousePos.getY() >= 0.0f &&
                      mousePos.getY() <= canvasHeight;

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
        if (_lastChangeTime >= constants::CHANGE_DEBOUNCE_TIME) {
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
    std::filesystem::path obstaclesPath = "configs/entities/obstacles";

    if (!std::filesystem::exists(obstaclesPath) ||
        !std::filesystem::is_directory(obstaclesPath)) {
        return obstacles;
    }

    for (const auto& entry : std::filesystem::directory_iterator(obstaclesPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            try {
                std::ifstream file(entry.path());
                nlohmann::json obstacleData;
                file >> obstacleData;
                file.close();

                if (obstacleData.contains("name") && obstacleData["name"].is_string()) {
                    std::string obstacleName = obstacleData["name"];
                    obstacles.push_back(obstacleName);
                }
            } catch (const std::exception&) {
            }
        }
    }

    return obstacles;
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
    /* musicDropdown will be added after background dropDown */

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
    _sidePanel->addChild(_backgroundDropdown);
    /* musicDropdown is added after background dropDown to ensure proper z order */
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
        if (_obstaclePrefabLabel) {
            _obstaclePrefabLabel->setVisible(showObstacles);
        }
        if (_obstaclePrefabDropdown) {
            _obstaclePrefabDropdown->setVisible(showObstacles);
        }
        if (_spritePreviewPanel) {
            _spritePreviewPanel->setVisible(showObstacles);
        }
        if (_spritePreview) {
            _spritePreview->setVisible(showObstacles);
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

    /* DropDowns are added at the end */
    _bottomPanel->addChild(_obstaclePrefabDropdown);
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
    _editorModeDropdown.reset();
    _obstaclePrefabLabel.reset();
    _obstaclePrefabDropdown.reset();
    _mouseHandler.reset();
    _uiManager.reset();
}

void LevelEditorState::saveToHistory() {
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
}

}  // namespace gsm
