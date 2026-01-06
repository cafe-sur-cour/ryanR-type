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
#include "../../base/AGameState.hpp"
#include "../../../../constants.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/elements/Panel.hpp"
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

    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Panel> _sidePanel;
    std::shared_ptr<ui::Panel> _bottomPanel;
    std::shared_ptr<ui::Panel> _canvasPanel;

    std::optional<std::filesystem::path> _levelPath;
};

}  // namespace gsm

#endif /* !LEVELEDITORSTATE_HPP_ */
