/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUIPowerUps
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

void LevelEditorState::createPowerUpUI() {
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
}

}  // namespace gsm
