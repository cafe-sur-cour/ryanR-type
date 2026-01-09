/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorHistory
*/

#include <vector>
#include <string>
#include "LevelEditorState.hpp"
#include "../../../../constants.hpp"

namespace gsm {

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

    _levelNameInput->setText(_levelData.value(constants::NAME_FIELD, "New Level"));
    _mapLengthInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::MAP_LENGTH_FIELD, 0.0))));
    _scrollSpeedInput->setText(std::to_string(
        static_cast<int>(_levelData.value(constants::BACKGROUND_SCROLL_SPEED_FIELD, 1))));

    std::string currentMusic = _levelData.value(constants::MUSIC_FIELD, "");
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

    std::string currentBackground = _levelData.value(constants::BACKGROUND_FIELD, "");
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

}  // namespace gsm
