/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorUIWaves
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

void LevelEditorState::createWaveUI() {
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
}

}  // namespace gsm
