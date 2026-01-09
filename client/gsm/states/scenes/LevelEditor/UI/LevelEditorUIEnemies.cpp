/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUIEnemies
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

void LevelEditorState::createEnemyUI() {
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
}

}  // namespace gsm
