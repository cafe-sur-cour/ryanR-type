/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#include "InGameState.hpp"
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <utility>
#include "../../../../../common/ECS/entity/Entity.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../Settings/SettingsState.hpp"
#include "../../../../../libs/Multimedia/IWindow.hpp"
#include "../../../../../libs/Multimedia/IEvent.hpp"
#include "../../../../../libs/Multimedia/IAudio.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../components/rendering/HitboxRenderComponent.hpp"
#include "../../../../systems/rendering/AnimationRenderingSystem.hpp"
#include "../../../../systems/rendering/HitboxRenderingSystem.hpp"
#include "../../../../systems/rendering/RectangleRenderingSystem.hpp"
#include "../../../../systems/rendering/TextRenderingSystem.hpp"
#include "../../../../systems/rendering/ParallaxRenderingSystem.hpp"
#include "../../../../systems/rendering/SpriteRenderingSystem.hpp"
#include "../../../../systems/rendering/HealthBarRenderingSystem.hpp"
#include "../../../../systems/input/MovementInputSystem.hpp"
#include "../../../../systems/input/ShootInputSystem.hpp"
#include "../../../../systems/audio/SoundSystem.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../../common/systems/shooting/ShootingSystem.hpp"
#include "../../../../../common/systems/lifetime/LifetimeSystem.hpp"
#include "../../../../../common/systems/death/DeathSystem.hpp"
#include "../../../../../common/systems/bounds/OutOfBoundsSystem.hpp"
#include "../../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../../common/constants.hpp"
#include "../../../../../common/Parser/CollisionRulesParser.hpp"
#include "../../../../../common/CollisionRules/CollisionRules.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../../../common/ECS/view/View.hpp"
#include "../../../../../common/components/tags/ObstacleTag.hpp"
#include "../../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../systems/rendering/GameZoneViewSystem.hpp"
#include "../../../../systems/audio/MusicSystem.hpp"
#include "../../../../components/temporary/MusicIntentComponent.hpp"
#include "../../../../../common/systems/ai/AIMovementSystem.hpp"
#include "../../../../../common/systems/ai/AIShootingSystem.hpp"
#include "../../../../../common/components/permanent/ScoreComponent.hpp"
#include "../../../../../common/components/permanent/HealthComponent.hpp"
#include "../../../../ClientNetwork.hpp"

namespace gsm {

InGameState::InGameState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager), _previousScore(-1), _previousHealth(-1) {
    _registry = resourceManager->get<ecs::Registry>();
    _prefabManager = resourceManager->get<EntityPrefabManager>();
    this->_parser = nullptr;
}

void InGameState::enter() {
    _resourceManager->add<EntityPrefabManager>(_prefabManager);
    _resourceManager->add<ecs::Registry>(_registry);

    auto collisionData =
        ecs::CollisionRulesParser::parseFromFile("configs/rules/collision_rules.json");
    ecs::CollisionRules::initWithData(collisionData);


    addSystem(std::make_shared<ecs::MovementInputSystem>());
    addSystem(std::make_shared<ecs::InputToVelocitySystem>());
    addSystem(std::make_shared<ecs::ShootInputSystem>());
    addSystem(std::make_shared<ecs::SoundSystem>());
    addSystem(std::make_shared<ecs::OutOfBoundsSystem>());
    addSystem(std::make_shared<ecs::GameZoneViewSystem>());
    addSystem(std::make_shared<ecs::MusicSystem>());
    addSystem(std::make_shared<ecs::ParallaxRenderingSystem>());
    addSystem(std::make_shared<ecs::SpriteRenderingSystem>());
    addSystem(std::make_shared<ecs::RectangleRenderingSystem>());
    addSystem(std::make_shared<ecs::AnimationRenderingSystem>());
    addSystem(std::make_shared<ecs::HitboxRenderingSystem>());
    addSystem(std::make_shared<ecs::HealthBarRenderingSystem>());
    addSystem(std::make_shared<ecs::TextRenderingSystem>());

    auto audio = _resourceManager->get<gfx::IAudio>();

    ecs::Entity musicIntentEntity = _registry->createEntity();
    _registry->addComponent<ecs::MusicIntentComponent>(musicIntentEntity,
        std::make_shared<ecs::MusicIntentComponent>(ecs::PLAY, ""));
    _resourceManager->get<ClientNetwork>()->sendWhoAmI();
}

void InGameState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        if (inputProvider->isActionPressed(ecs::InputAction::MENU_BACK)) {
            if (auto stateMachine = _gsm.lock()) {
                stateMachine->requestStatePush(std::make_shared<SettingsState>(stateMachine,
                    _resourceManager));
                return;
            }
        }
    }

    _resourceManager->get<ecs::ISystemManager>()->updateAllSystems
        (_resourceManager, _registry, deltaTime);

    for (auto it = _scoreFeedbacks.begin(); it != _scoreFeedbacks.end(); ) {
        it->lifetime -= deltaTime;
        if (it->lifetime <= 0.0f) {
            it = _scoreFeedbacks.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = _healthFeedbacks.begin(); it != _healthFeedbacks.end(); ) {
        it->lifetime -= deltaTime;
        if (it->lifetime <= 0.0f) {
            it = _healthFeedbacks.erase(it);
        } else {
            ++it;
        }
    }

    renderHUD();
}

void InGameState::renderHUD() {
    /* This HUD is a showcase example. It's indeed very basic and should be */
    /* Proper HUD will be implented later in the Game Scene states.         */

    auto window = _resourceManager->get<gfx::IWindow>();
    auto currentCenter = window->getViewCenter();
    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);

    gfx::color_t blackSemiTransparent = {0, 0, 0, 128};
    std::pair<size_t, size_t> position = {0, static_cast<size_t>(constants::MAX_HEIGHT - 50)};
    std::pair<size_t, size_t> size = {static_cast<size_t>(constants::MAX_WIDTH), 50};

    window->drawFilledRectangle(blackSemiTransparent, position, size);

    int score = 0;
    float health = 0.0f;
    float maxHealth = 0.0f;
    auto view = _registry->view<ecs::PlayerTag, ecs::LocalPlayerTag>();
    for (auto entity : view) {
        if (_registry->hasComponent<ecs::ScoreComponent>(entity)) {
            auto scoreComp = _registry->getComponent<ecs::ScoreComponent>(entity);
            score = scoreComp->getScore();
        }
        if (_registry->hasComponent<ecs::HealthComponent>(entity)) {
            auto healthComp = _registry->getComponent<ecs::HealthComponent>(entity);
            health = healthComp->getHealth();
            maxHealth = healthComp->getBaseHealth();
        }
        break;
    }

    if (_previousHealth == -1) {
        _previousHealth = static_cast<int>(health);
    } else if (static_cast<int>(health) < _previousHealth) {
        int lost = _previousHealth - static_cast<int>(health);
        _healthFeedbacks.push_back({"-" + std::to_string(lost), 1.0f, 1.0f});
        _previousHealth = static_cast<int>(health);
    } else if (static_cast<int>(health) > _previousHealth) {
        _previousHealth = static_cast<int>(health);
    }

    if (_previousScore == -1) {
        _previousScore = score;
    } else if (score > _previousScore) {
        int gained = score - _previousScore;
        _scoreFeedbacks.push_back({"+" + std::to_string(gained), 1.0f, 1.0f});
        _previousScore = score;
    } else if (score < _previousScore) {
        _previousScore = score;
    }

    std::stringstream healthSs;
    healthSs << "Health: " << static_cast<int>(health) << "/" << static_cast<int>(maxHealth);
    std::string healthText = healthSs.str();

    std::stringstream scoreSs;
    scoreSs << "Score: " << std::setfill('0') << std::setw(5) << score;
    std::string scoreText = scoreSs.str();

    gfx::color_t white = {255, 255, 255, 255};
    std::pair<size_t, size_t> healthPosition =
        {10, static_cast<size_t>(constants::MAX_HEIGHT - 35)};
    window->drawText(healthText, white, healthPosition, "assets/fonts/arial.ttf", 24);

    std::pair<size_t, size_t> scorePosition =
        {200, static_cast<size_t>(constants::MAX_HEIGHT - 35)};
    window->drawText(scoreText, white, scorePosition, "assets/fonts/arial.ttf", 24);

    for (const auto& feedback : _healthFeedbacks) {
        uint8_t alpha =
            static_cast<uint8_t>((feedback.lifetime / feedback.maxLifetime) * 255.0f);
        gfx::color_t red = {255, 0, 0, alpha};
        size_t x = 10;
        size_t base_y = constants::MAX_HEIGHT - 35 - 25;
        float progress = 1.0f - (feedback.lifetime / feedback.maxLifetime);
        size_t y = base_y - static_cast<size_t>(progress * 50.0f);
        std::pair<size_t, size_t> feedbackPosition = {x, y};
        window->drawText(feedback.text, red, feedbackPosition, "assets/fonts/arial.ttf", 28);
    }

    for (const auto& feedback : _scoreFeedbacks) {
        uint8_t alpha =
            static_cast<uint8_t>((feedback.lifetime / feedback.maxLifetime) * 255.0f);
        gfx::color_t green = {0, 255, 0, alpha};
        size_t x = 200;
        size_t base_y = constants::MAX_HEIGHT - 35 - 25;
        float progress = 1.0f - (feedback.lifetime / feedback.maxLifetime);
        size_t y = base_y - static_cast<size_t>(progress * 50.0f);
        std::pair<size_t, size_t> feedbackPosition = {x, y};
        window->drawText(feedback.text, green, feedbackPosition, "assets/fonts/arial.ttf", 28);
    }

    window->setViewCenter(currentCenter.getX(), currentCenter.getY());
}

void InGameState::exit() {
}

}  // namespace gsm
