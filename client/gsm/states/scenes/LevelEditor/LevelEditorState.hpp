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
    std::string type;
    int index;
};

struct PowerUpData {
    float posX;
    float posY;
};

struct PowerUpSelection {
    std::string prefabName;
    int index;
};

struct WaveDistribution {
    float min;
    float max;
    std::string type;
};

struct WaveEnemy {
    std::string type;
    WaveDistribution distributionX;
    WaveDistribution distributionY;
    int count;
};

struct Wave {
    float gameXTrigger;
    std::vector<WaveEnemy> enemies;
};

struct WaveSelection {
    int waveIndex;
    int enemyIndex;
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

    bool isAnimation = false;
    float frameCount = 0.0f;
    float frameWidth = 0.0f;
    float frameHeight = 0.0f;
    float animationSpeed = 0.1f;
    bool animationLoop = true;
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
    std::vector<std::string> loadAvailablePowerUps();
    std::vector<std::string> loadAvailableEnemies();
    void saveToHistory();
    void loadFromHistory(size_t index);
    void updateHistoryButtons();
    void initializeViewport();
    void handleZoom(float deltaTime, gfx::EventType eventResult);
    void handleCanvasDrag(float deltaTime);
    void renderLevelPreview();
    void renderSpriteInLevelPreview(const LevelPreviewSprite& spriteData, const std::string& prefabName, float screenX, float screenY, float canvasLeft, float canvasRight, float canvasTop, float canvasBottom);
    LevelPreviewSprite extractSpriteDataFromPrefab(const std::string& prefabPath);

    /* Obstacles methods */
    void parseObstacles();
    void renderAllObstacles(float levelX, float levelY, float canvasLeft, float canvasRight, float canvasTop, float canvasBottom);
    void saveObstacles();
    void handleObstacleClick(float mouseX, float mouseY, float levelX, float levelY);
    void startObstacleDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    void handleObstacleDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    std::optional<ObstacleSelection> getObstacleAtPosition(float mouseX, float mouseY, float levelX, float levelY);

    /* PowerUps methods */
    void parsePowerUps();
    void renderAllPowerUps(float levelX, float levelY, float canvasLeft, float canvasRight, float canvasTop, float canvasBottom);
    void savePowerUps();
    void handlePowerUpClick(float mouseX, float mouseY, float levelX, float levelY);
    void startPowerUpDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    void handlePowerUpDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    std::optional<PowerUpSelection> getPowerUpAtPosition(float mouseX, float mouseY, float levelX, float levelY);

    /* Waves methods */
    void parseWaves();
    void renderAllWaves(float levelX, float levelY, float canvasLeft, float canvasRight, float canvasTop, float canvasBottom);
    void saveWaves();
    void handleWaveClick(float mouseX, float mouseY, float levelX, float levelY);
    void startWaveDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    void handleWaveDrag(math::Vector2f mousePos, float viewportZoom, float sidePanelWidth);
    std::optional<WaveSelection> getWaveAtPosition(float mouseX, float mouseY, float levelX, float levelY);
    void updateEnemyUI();

    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Panel> _sidePanel;
    std::shared_ptr<ui::Panel> _bottomPanel;
    std::shared_ptr<ui::Panel> _canvasPanel;
    std::shared_ptr<ui::Dropdown> _editorModeDropdown;
    std::shared_ptr<ui::Text> _obstaclePrefabLabel;
    std::shared_ptr<ui::Dropdown> _obstaclePrefabDropdown;
    std::shared_ptr<ui::Text> _powerUpPrefabLabel;
    std::shared_ptr<ui::Dropdown> _powerUpPrefabDropdown;
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
    std::shared_ptr<ui::Button> _filterButton;
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
    std::shared_ptr<ui::Button> _obstacleDuplicateButton;
    std::shared_ptr<ui::Text> _powerUpPosXLabel;
    std::shared_ptr<ui::TextInput> _powerUpPosXInput;
    std::shared_ptr<ui::Text> _powerUpPosYLabel;
    std::shared_ptr<ui::TextInput> _powerUpPosYInput;
    std::shared_ptr<ui::Button> _powerUpDeleteButton;
    std::shared_ptr<ui::Button> _powerUpDuplicateButton;
    std::shared_ptr<ui::Text> _waveLabel;
    std::shared_ptr<ui::Text> _waveIndexLabel;
    std::shared_ptr<ui::Button> _wavePrevButton;
    std::shared_ptr<ui::Button> _waveNextButton;
    std::shared_ptr<ui::Button> _waveDeleteButton;
    std::shared_ptr<ui::Button> _waveDuplicateButton;
    std::shared_ptr<ui::Text> _waveTriggerXLabel;
    std::shared_ptr<ui::TextInput> _waveTriggerXInput;

    // Enemy UI elements
    std::shared_ptr<ui::Text> _enemyLabel;
    std::shared_ptr<ui::Text> _enemyIndexLabel;
    std::shared_ptr<ui::Button> _enemyPrevButton;
    std::shared_ptr<ui::Button> _enemyNextButton;
    std::shared_ptr<ui::Button> _enemyAddButton;
    std::shared_ptr<ui::Button> _enemyDeleteButton;
    std::shared_ptr<ui::Text> _enemyTypeLabel;
    std::shared_ptr<ui::TextInput> _enemyTypeInput;
    std::shared_ptr<ui::Button> _enemyApplyTypeButton;
    std::shared_ptr<ui::Text> _enemyAppliedTypeLabel;
    std::shared_ptr<ui::Text> _enemyCountLabel;
    std::shared_ptr<ui::TextInput> _enemyCountInput;
    std::shared_ptr<ui::Text> _enemyDistXMinLabel;
    std::shared_ptr<ui::TextInput> _enemyDistXMinInput;
    std::shared_ptr<ui::Text> _enemyDistXMaxLabel;
    std::shared_ptr<ui::TextInput> _enemyDistXMaxInput;
    std::shared_ptr<ui::Text> _enemyDistYMinLabel;
    std::shared_ptr<ui::TextInput> _enemyDistYMinInput;
    std::shared_ptr<ui::Text> _enemyDistYMaxLabel;
    std::shared_ptr<ui::TextInput> _enemyDistYMaxInput;
    std::shared_ptr<ui::Text> _enemyDistXTypeLabel;
    std::shared_ptr<ui::Dropdown> _enemyDistXTypeDropdown;
    std::shared_ptr<ui::Text> _enemyDistYTypeLabel;
    std::shared_ptr<ui::Dropdown> _enemyDistYTypeDropdown;

    bool _hasUnsavedChanges = false;
    bool _showHitboxes = false;
    std::string _displayFilter = "All"; // "All", "Obstacles", "PowerUps", "Waves"

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
    bool _isDraggingPowerUp = false;
    math::Vector2f _dragPowerUpOffset;
    bool _isDraggingWave = false;
    float _dragWaveOffsetX = 0.0f;

    std::map<std::string, ObstacleGroup> _obstaclesByName;
    std::optional<ObstacleSelection> _selectedObstacle;
    std::map<std::string, std::vector<PowerUpData>> _powerUpsByName;
    std::optional<PowerUpSelection> _selectedPowerUp;
    std::vector<Wave> _waves;
    std::optional<WaveSelection> _selectedWave;
    int _currentWaveIndex = 0;
    int _currentEnemyIndex = 0;
    std::vector<std::string> _availableEnemies;

    std::map<std::string, LevelPreviewSprite> _obstacleAnimationData;
    std::map<std::string, float> _obstacleAnimationFrames;
    std::map<std::string, float> _obstacleAnimationTimes;
    std::map<std::string, LevelPreviewSprite> _powerUpAnimationData;
    std::map<std::string, float> _powerUpAnimationFrames;
    std::map<std::string, float> _powerUpAnimationTimes;

};

}  // namespace gsm


#endif /* !LEVELEDITORSTATE_HPP_ */
