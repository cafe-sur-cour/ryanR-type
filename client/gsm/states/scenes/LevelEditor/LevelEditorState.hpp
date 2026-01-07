/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorState
*/

#ifndef LEVELEDITORSTATE_HPP_
#define LEVELEDITORSTATE_HPP_

#include "../../base/AGameState.hpp"
#include <filesystem>  // NOLINT(build/c++17)
#include <nlohmann/json.hpp>
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../SettingsConfig.hpp"

namespace gsm {

class LevelEditorState : public AGameState {
public:
    LevelEditorState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~LevelEditorState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();
    void createLevelSelectionUI();
    std::vector<std::pair<std::filesystem::path, int>> getAvailableLevels();
    void swapLevels(const std::filesystem::path& path1, const std::filesystem::path& path2);
    void showDeleteConfirmation(const std::filesystem::path& levelPath, const std::string& levelName);
    void showDeleteConfirmationPopup(const std::filesystem::path& levelPath, const std::string& levelName);
    void hideDeleteConfirmationPopup();
    void confirmDelete();
    void showDuplicateConfirmation(const std::filesystem::path& levelPath, const std::string& levelName);
    void showDuplicateConfirmationPopup(const std::filesystem::path& levelPath, const std::string& levelName);
    void hideDuplicateConfirmationPopup();
    void confirmDuplicate();
    void setMainButtonsEnabled(bool enabled);

    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Button> _backButton;
    std::vector<std::shared_ptr<ui::Button>> _levelButtons;
    std::vector<std::shared_ptr<ui::Text>> _indexLabels;
    std::shared_ptr<ui::Button> _addLevelButton;
    std::vector<std::shared_ptr<ui::Button>> _upButtons;
    std::vector<std::shared_ptr<ui::Button>> _downButtons;
    std::vector<std::shared_ptr<ui::Button>> _duplicateButtons;
    std::vector<std::shared_ptr<ui::Button>> _deleteButtons;
    std::shared_ptr<ui::Button> _prevButton;
    std::shared_ptr<ui::Button> _nextButton;
    bool _shouldUpdateUI;
    int _currentPage;
    static constexpr int _levelsPerPage = 8;

    std::shared_ptr<ui::UILayout> _deletePopupOverlay;
    std::shared_ptr<ui::UILayout> _deletePopupLayout;
    std::shared_ptr<ui::Text> _deletePopupText;
    std::shared_ptr<ui::Button> _deleteCancelButton;
    std::shared_ptr<ui::Button> _deleteConfirmButton;
    std::filesystem::path _pendingDeletePath;
    bool _shouldHideDeletePopup;

    std::shared_ptr<ui::UILayout> _duplicatePopupOverlay;
    std::shared_ptr<ui::UILayout> _duplicatePopupLayout;
    std::shared_ptr<ui::Text> _duplicatePopupText;
    std::shared_ptr<ui::Button> _duplicateCancelButton;
    std::shared_ptr<ui::Button> _duplicateConfirmButton;
    std::filesystem::path _pendingDuplicatePath;
    std::string _pendingDuplicateName;
    bool _shouldHideDuplicatePopup;
};

}  // namespace gsm

#endif /* !LEVELEDITORSTATE_HPP_ */