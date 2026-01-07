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
#include <map>
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
#include "../../../../ui/elements/SpritePreview.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

struct HorizontalLineObstacle {
    float fromX;
    float posY;
    int count;
};

struct VerticalLineObstacle {
    float fromY;
    float posX;
    int count;
};

struct UniqueObstacle {
    float posX;
    float posY;
};

struct ObstacleSelection {
    std::string prefabName;
    std::string type;  // "unique", "horizontal", "vertical"
    int index;  // index dans le vecteur correspondant
};

struct ObstacleGroup {
    std::vector<HorizontalLineObstacle> horizontalLines;
    std::vector<VerticalLineObstacle> verticalLines;
    std::vector<UniqueObstacle> uniques;
};

struct LevelPreviewSprite {
    std::string texturePath;
    float width;
    float height;
    float posX;
    float posY;
    float scale;
    float rotation;
};

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
    void createBottomPanel();
    void updateSaveButtonText();
    bool validateFields();
    std::vector<std::string> loadAvailableMusics();
    std::vector<std::string> loadAvailableBackgrounds();
    std::vector<std::string> loadAvailableObstacles();
    void saveToHistory();
    void loadFromHistory(size_t index);
    void updateHistoryButtons();
    void initializeViewport();
    void handleZoom(float deltaTime, gfx::EventType eventResult);
    void handleCanvasDrag(float deltaTime);
    void renderLevelPreview();
    void renderSpriteInLevelPreview(const LevelPreviewSprite& spriteData, float screenX, float screenY, float canvasLeft, float canvasRight, float canvasTop, float canvasBottom);
    LevelPreviewSprite extractSpriteDataFromPrefab(const std::string& prefabPath);

    /* Obstacles methods */
    void parseObstacles();
    void renderAllObstacles(float levelX, float levelY, float canvasLeft, float canvasRight, float canvasTop, float canvasBottom);
    void saveObstacles();
    void handleObstacleClick(float mouseX, float mouseY, float levelX, float levelY);
    void startObstacleDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    void handleObstacleDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    std::optional<ObstacleSelection> getObstacleAtPosition(float mouseX, float mouseY, float levelX, float levelY);

    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Panel> _sidePanel;
    std::shared_ptr<ui::Panel> _bottomPanel;
    std::shared_ptr<ui::Panel> _canvasPanel;
    std::shared_ptr<ui::Dropdown> _editorModeDropdown;
    std::shared_ptr<ui::Text> _obstaclePrefabLabel;
    std::shared_ptr<ui::Dropdown> _obstaclePrefabDropdown;
    std::shared_ptr<ui::Panel> _spritePreviewPanel;
    std::shared_ptr<ui::SpritePreview> _spritePreview;
    std::shared_ptr<ui::Text> _spriteWidthLabel;
    std::shared_ptr<ui::Text> _spriteHeightLabel;
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
    std::shared_ptr<ui::Button> _showHitboxesButton;
    std::shared_ptr<ui::Text> _obstacleTypeLabel;
    std::shared_ptr<ui::Dropdown> _obstacleTypeDropdown;
    std::shared_ptr<ui::Text> _obstaclePosXLabel;
    std::shared_ptr<ui::TextInput> _obstaclePosXInput;
    std::shared_ptr<ui::Text> _obstaclePosYLabel;
    std::shared_ptr<ui::TextInput> _obstaclePosYInput;
    std::shared_ptr<ui::Text> _obstacleCountLabel;
    std::shared_ptr<ui::TextInput> _obstacleCountInput;
    std::shared_ptr<ui::Button> _obstacleDeleteButton;

    bool _hasUnsavedChanges = false;
    bool _showHitboxes = false;

    std::optional<std::filesystem::path> _levelPath;
    nlohmann::json _levelData;

    std::vector<nlohmann::json> _history;
    size_t _currentHistoryIndex = 0;
    float _lastChangeTime = constants::CHANGE_DEBOUNCE_TIME + 1.0f;
    bool _hasPendingChange = false;
    bool _isLoadingFromHistory = false;
    bool _undoPressedLastFrame = false;
    bool _redoPressedLastFrame = false;
    bool _leftMousePressedLastFrame = false;

    math::Vector2f _viewportOffset;
    float _viewportZoom = 1.0f;
    float _minZoom = 0.1f;
    float _maxZoom = 2.0f;

    bool _isDragging = false;
    math::Vector2f _lastMousePos;
    math::Vector2f _dragStartPos;
    bool _isDraggingObstacle = false;
    math::Vector2f _dragObstacleOffset;

    std::map<std::string, ObstacleGroup> _obstaclesByName;
    std::optional<ObstacleSelection> _selectedObstacle;

};

}  // namespace gsm


#endif /* !LEVELEDITORSTATE_HPP_ */
