/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorState
*/

#ifndef LEVELEDITORSTATE_HPP_
#define LEVELEDITORSTATE_HPP_

#include <filesystem>  // NOLINT(build/c++17)
#include <optional>
#include <nlohmann/json.hpp>
#include "../../base/AGameState.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/Panel.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../../libs/Multimedia/EventTypes.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/focusable/TextInput.hpp"
#include "../../../../ui/elements/focusable/Dropdown.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

class LevelEditorState : public AGameState {
public:
    LevelEditorState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager, std::optional<std::filesystem::path> levelPath = std::nullopt);
    ~LevelEditorState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();
    void createUI();
    void updateSaveButtonText();
    bool validateFields();
    std::vector<std::string> loadAvailableMusics();
    std::vector<std::string> loadAvailableBackgrounds();
    void saveToHistory();
    void loadFromHistory(size_t index);
    void updateHistoryButtons();
    void initializeViewport();
    void handleZoom(float deltaTime, gfx::EventType eventResult);
    void handleCanvasDrag(float deltaTime);
    void renderLevelPreview();


    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Panel> _sidePanel;
    std::shared_ptr<ui::Panel> _bottomPanel;
    std::shared_ptr<ui::Panel> _canvasPanel;
    std::shared_ptr<ui::Button> _saveButton;
    std::shared_ptr<ui::Button> _backButton;
    std::shared_ptr<ui::Text> _nameLabel;
    std::shared_ptr<ui::TextInput> _levelNameInput;
    std::shared_ptr<ui::Text> _mapLengthLabel;
    std::shared_ptr<ui::TextInput> _mapLengthInput;
    std::shared_ptr<ui::Text> _scrollSpeedLabel;
    std::shared_ptr<ui::TextInput> _scrollSpeedInput;
    std::shared_ptr<ui::Text> _musicLabel;
    std::shared_ptr<ui::Dropdown> _musicDropdown;
    std::shared_ptr<ui::Text> _backgroundLabel;
    std::shared_ptr<ui::Dropdown> _backgroundDropdown;
    std::shared_ptr<ui::Button> _undoButton;
    std::shared_ptr<ui::Button> _redoButton;
    std::shared_ptr<ui::Text> _cursorPosLabel;
    std::shared_ptr<ui::Text> _cursorPosYLabel;
    std::shared_ptr<ui::Button> _resetViewButton;

    bool _hasUnsavedChanges = false;

    std::optional<std::filesystem::path> _levelPath;
    nlohmann::json _levelData;

    std::vector<nlohmann::json> _history;
    size_t _currentHistoryIndex = 0;
    float _lastChangeTime = constants::CHANGE_DEBOUNCE_TIME + 1.0f;
    bool _hasPendingChange = false;
    bool _isLoadingFromHistory = false;
    bool _undoPressedLastFrame = false;
    bool _redoPressedLastFrame = false;

    math::Vector2f _viewportOffset;
    float _viewportZoom = 1.0f;
    float _minZoom = 0.1f;
    float _maxZoom = 2.0f;

    bool _isDragging = false;
    math::Vector2f _lastMousePos;
    math::Vector2f _dragStartPos;

};

}  // namespace gsm


#endif /* !LEVELEDITORSTATE_HPP_ */
