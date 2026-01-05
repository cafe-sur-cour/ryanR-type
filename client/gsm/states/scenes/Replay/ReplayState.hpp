/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ReplayState
*/

#ifndef REPLAYSTATE_HPP_
#define REPLAYSTATE_HPP_

#include "../../base/AGameState.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../input/MouseInputHandler.hpp"
#include "../../../../ui/elements/focusable/Button.hpp"
#include "../../../../ui/elements/Text.hpp"
#include "../../../../ui/elements/Background.hpp"
#include "../../../../ui/manager/UIManager.hpp"
#include "../../../../ui/core/UILayout.hpp"
#include "../../../../../common/types/Vector2f.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../components/temporary/MusicIntentComponent.hpp"
#include "../../../../SettingsConfig.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>

namespace gsm {

class ReplayState : public AGameState {
public:
    ReplayState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
    ~ReplayState() override = default;

    void enter() override;
    void update(float deltaTime) override;
    void exit() override;

private:
    void renderUI();
    void renderReplaySprites();
    void renderParallaxBackground(const nlohmann::json& parallaxData, std::shared_ptr<gfx::IWindow> window);
    void renderHealthBar(const nlohmann::json& healthBarData, std::shared_ptr<gfx::IWindow> window);
    void renderText(const nlohmann::json& textData, std::shared_ptr<gfx::IWindow> window);
    void renderRectangle(const nlohmann::json& rectangleData, std::shared_ptr<gfx::IWindow> window);
    void renderHitbox(const nlohmann::json& hitboxData, std::shared_ptr<gfx::IWindow> window);
    void updateViewForFrame(const nlohmann::json& frame);
    void loadReplay(const std::filesystem::path& replayFile);
    void playReplay(float deltaTime);
    void processAudioForFrame(const nlohmann::json& frame);
    std::vector<std::filesystem::path> getAvailableReplays();
    void createReplaySelectionUI();

    std::unique_ptr<MouseInputHandler> _mouseHandler;
    std::unique_ptr<ui::UIManager> _uiManager;

    std::shared_ptr<ui::Background> _background;
    std::shared_ptr<ui::Button> _backButton;
    std::shared_ptr<ui::Text> _statusText;
    std::vector<std::shared_ptr<ui::Button>> _replayButtons;

    std::vector<nlohmann::json> _frames;
    float _replayTime;
    float _totalReplayTime;
    size_t _currentFrameIndex;
    bool _isPlaying;
};

}  // namespace gsm

#endif /* !REPLAYSTATE_HPP_ */
