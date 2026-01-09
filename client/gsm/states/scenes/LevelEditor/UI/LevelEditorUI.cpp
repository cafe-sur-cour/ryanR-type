/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUI
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

        _levelData[constants::NAME_FIELD] = levelName;

        try {
            float mapLength = std::stof(mapLengthStr);
            _levelData[constants::MAP_LENGTH_FIELD] = mapLength;
        } catch (const std::exception&) {
        }

        try {
            int scrollSpeed = std::stoi(scrollSpeedStr);
            _levelData[constants::BACKGROUND_SCROLL_SPEED_FIELD] = scrollSpeed;
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
    _levelNameInput->setText(_levelData.value(constants::NAME_FIELD, "New Level"));
    _levelNameInput->setPlaceholder("Enter level name...");
    _levelNameInput->setOnTextChanged([this](const std::string& text) {
        if (_isLoadingFromHistory) {
            return;
        }
        _levelData[constants::NAME_FIELD] = text;
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
        static_cast<int>(_levelData.value(constants::MAP_LENGTH_FIELD, 0.0))));
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
            _levelData[constants::MAP_LENGTH_FIELD] = mapLength;
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
        static_cast<int>(_levelData.value(constants::BACKGROUND_SCROLL_SPEED_FIELD, 1))));
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
            _levelData[constants::BACKGROUND_SCROLL_SPEED_FIELD] = scrollSpeed;
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

    std::string currentMusic = _levelData.value(constants::MUSIC_FIELD, "");
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
        _levelData[constants::MUSIC_FIELD] = musicName;
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

    std::string currentBackground = _levelData.value(constants::BACKGROUND_FIELD, "");
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
        _levelData[constants::BACKGROUND_FIELD] = backgroundName;
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

}  // namespace gsm
