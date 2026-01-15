/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#ifndef INGAMESTATE_HPP_
#define INGAMESTATE_HPP_

#include "../../base/AGameState.hpp"
#include "resourceManager/ResourceManager.hpp"
#include "../../../../../common/Prefab/entityPrefabManager/EntityPrefabManager.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include <vector>
#include <string>
#include <memory>

namespace gsm {

struct ScoreFeedback {
    std::string text;
    float lifetime;
    float maxLifetime;
};

class InGameState : public AGameState {
    public:
        InGameState(std::shared_ptr<IGameStateMachine> gsm, std::shared_ptr<ResourceManager> resourceManager);
        ~InGameState() override = default;

        void enter() override;
        void update(float deltaTime) override;
        void exit() override;
        std::string getStateName() const override { return "In Game"; }

    private:
        void renderHUD(float deltaTime);
        void drawHealthHUD(std::shared_ptr<gfx::IWindow> window, float health, float maxHealth);
        void drawScoreHUD(std::shared_ptr<gfx::IWindow> window, int score);
        void drawShotChargeHUD(std::shared_ptr<gfx::IWindow> window, float shotCharge, float maxShotCharge);
        void drawInGameMetrics(std::shared_ptr<gfx::IWindow> window, float deltaTime);

    private:
        std::shared_ptr<ecs::Registry> _registry;
        std::shared_ptr<EntityPrefabManager> _prefabManager;
        std::shared_ptr<Parser> _parser;
        int _previousScore;
        int _previousHealth;
        std::vector<ScoreFeedback> _scoreFeedbacks;
        std::vector<ScoreFeedback> _healthFeedbacks;
        float _whoAmITimer = 0.0f;
        bool _localPlayerFound = false;
};

}  // namespace gsm

#endif  // INGAMESTATE_HPP_
