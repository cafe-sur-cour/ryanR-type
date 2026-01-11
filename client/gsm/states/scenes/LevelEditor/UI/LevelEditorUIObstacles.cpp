/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUIObstacles
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

void LevelEditorState::createObstacleUI() {
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
    _bottomPanel->addChild(_obstaclePrefabDropdown);
}

}  // namespace gsm
