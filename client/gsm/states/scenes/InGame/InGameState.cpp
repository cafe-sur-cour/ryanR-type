/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InGameState
*/

#include "InGameState.hpp"
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <unistd.h>
#endif
#include <memory>
#include <iostream>
#include <sstream>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <utility>
#include <algorithm>
#include "../Settings/SettingsState.hpp"
#include "../Pause/PauseState.hpp"
#include "../../../../systems/rendering/AnimationRenderingSystem.hpp"
#include "../../../../systems/rendering/HitboxRenderingSystem.hpp"
#include "../../../../systems/rendering/RectangleRenderingSystem.hpp"
#include "../../../../systems/rendering/TextRenderingSystem.hpp"
#include "../../../../systems/rendering/ParallaxRenderingSystem.hpp"
#include "../../../../systems/rendering/SpriteRenderingSystem.hpp"
#include "../../../../systems/rendering/HealthBarRenderingSystem.hpp"
#include "../../../../systems/rendering/GameZoneViewSystem.hpp"
#include "../../../../systems/input/MovementInputSystem.hpp"
#include "../../../../systems/input/ShootInputSystem.hpp"
#include "../../../../systems/input/ForceInputSystem.hpp"
#include "../../../../systems/audio/SoundSystem.hpp"
#include "../../../../systems/animationState/AnimationStateSyncSystem.hpp"
#include "../../../../systems/replay/ReplaySystem.hpp"
#include "../../../../systems/audio/MusicSystem.hpp"
#include "../../../../systems/network/NetworkInterpolationSystem.hpp"
#include "../../../../systems/effects/ClientEffectCleanupSystem.hpp"
#include "../../../../systems/effects/HideLifetimeSystem.hpp"
#include "../../../../components/temporary/MusicIntentComponent.hpp"
#include "../../../../components/rendering/HitboxRenderComponent.hpp"
#include "../../../../components/permanent/NetworkStateComponent.hpp"
#include "../../../../../common/components/permanent/VelocityComponent.hpp"
#include "../../../../colors.hpp"
#include "../../../../ClientNetwork.hpp"
#include "../../../../../common/ECS/entity/Entity.hpp"
#include "../../../../../common/ECS/view/View.hpp"
#include "../../../../../common/CollisionRules/CollisionRules.hpp"
#include "../../../../../common/interfaces/IWindow.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../../common/interfaces/IAudio.hpp"
#include "../../../../../common/InputMapping/IInputProvider.hpp"
#include "../../../../../common/systems/movement/MovementSystem.hpp"
#include "../../../../../common/systems/movement/InputToVelocitySystem.hpp"
#include "../../../../../common/systems/death/DeathSystem.hpp"
#include "../../../../../common/systems/bounds/OutOfBoundsSystem.hpp"
#include "../../../../../common/systems/health/HealthSystem.hpp"
#include "../../../../../common/systems/score/ScoreSystem.hpp"
#include "../../../../../common/systems/interactions/TriggerSystem.hpp"
#include "../../../../../common/systems/interactions/InteractionSystem.hpp"
#include "../../../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../../common/components/tags/PlayerTag.hpp"
#include "../../../../../common/components/tags/LocalPlayerTag.hpp"
#include "../../../../../common/components/tags/ObstacleTag.hpp"
#include "../../../../../common/components/permanent/ScoreComponent.hpp"
#include "../../../../../common/components/permanent/HealthComponent.hpp"
#include "../../../../../common/components/permanent/ChargedShotComponent.hpp"
#include "../../../../../common/gsm/IGameStateMachine.hpp"
#include "../../../../../common/Parser/CollisionRulesParser.hpp"
#include "../../../../../common/Parser/Parser.hpp"
#include "../../../../../common/constants.hpp"

namespace gsm {

InGameState::InGameState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager)
    : AGameState(gsm, resourceManager), _previousScore(-1), _previousHealth(-1) {
    _registry = resourceManager->get<ecs::Registry>();
    if (resourceManager->has<Parser>()) {
        _prefabManager = resourceManager->get<Parser>()->getPrefabManager();
    } else {
        _prefabManager = resourceManager->get<EntityPrefabManager>();
    }
    this->_parser = nullptr;
}

void InGameState::enter() {
    if (_resourceManager->has<gfx::IAudio>()) {
        auto audio = _resourceManager->get<gfx::IAudio>();
        audio->stopMusic();
    }

    _resourceManager->add<EntityPrefabManager>(_prefabManager);
    _resourceManager->add<ecs::Registry>(_registry);

    if (_resourceManager->has<gfx::IWindow>()) {
        auto window = _resourceManager->get<gfx::IWindow>();
        window->setViewCenter(
            constants::MAX_WIDTH / 2.0f,
            constants::MAX_HEIGHT / 2.0f
        );
    }

    auto collisionData =
        ecs::CollisionRulesParser::parseFromFile(constants::COLLISION_RULES_PATH);
    ecs::CollisionRules::initWithData(collisionData);

    auto localPlayerView = _registry->view<ecs::PlayerTag, ecs::LocalPlayerTag>();
    if (!(localPlayerView.begin() == localPlayerView.end())) {
        auto localPlayer = *localPlayerView.begin();
        _registry->addComponent(localPlayer, std::make_shared<ecs::NetworkStateComponent>());
    }

    auto systemManager = _resourceManager->get<ecs::ISystemManager>();
    systemManager->clearAllSystems();

    addSystem(std::make_shared<ecs::NetworkInterpolationSystem>());
    addSystem(std::make_shared<ecs::MovementInputSystem>());
    addSystem(std::make_shared<ecs::InputToVelocitySystem>());
    addSystem(std::make_shared<ecs::ShootInputSystem>());
    addSystem(std::make_shared<ecs::ForceInputSystem>());
    addSystem(std::make_shared<ecs::AnimationStateSyncSystem>());
    addSystem(std::make_shared<ecs::OutOfBoundsSystem>());
    addSystem(std::make_shared<ecs::ClientEffectCleanupSystem>());
    addSystem(std::make_shared<ecs::GameZoneViewSystem>());
    addSystem(std::make_shared<ecs::ParallaxRenderingSystem>());
    addSystem(std::make_shared<ecs::SpriteRenderingSystem>());
    addSystem(std::make_shared<ecs::RectangleRenderingSystem>());
    addSystem(std::make_shared<ecs::AnimationRenderingSystem>());
    addSystem(std::make_shared<ecs::HitboxRenderingSystem>());
    addSystem(std::make_shared<ecs::HealthBarRenderingSystem>());
    addSystem(std::make_shared<ecs::TextRenderingSystem>());
    addSystem(std::make_shared<ecs::ReplaySystem>());
    addSystem(std::make_shared<ecs::SoundSystem>());
    addSystem(std::make_shared<ecs::MusicSystem>());
    addSystem(std::make_shared<ecs::HideLifetimeSystem>());

    auto audio = _resourceManager->get<gfx::IAudio>();

    ecs::Entity musicIntentEntity = _registry->createEntity();
    _registry->addComponent<ecs::MusicIntentComponent>(musicIntentEntity,
        std::make_shared<ecs::MusicIntentComponent>(ecs::PLAY, ""));
}

void InGameState::update(float deltaTime) {
    auto localPlayerView = _registry->view<ecs::PlayerTag, ecs::LocalPlayerTag>();
    if (!(localPlayerView.begin() == localPlayerView.end())) {
        auto localPlayer = *localPlayerView.begin();
        if (!(_registry->hasComponent<ecs::NetworkStateComponent>(localPlayer)))
            _registry->addComponent(
                localPlayer, std::make_shared<ecs::NetworkStateComponent>());
        _localPlayerFound = true;
    }

    if (!_localPlayerFound) {
        _whoAmITimer += deltaTime;
        if (_whoAmITimer >= constants::WHOAMI_REQUEST_INTERVAL) {
            _resourceManager->get<ClientNetwork>()->sendWhoAmI();
            _whoAmITimer = 0.0f;
        }
    }

    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    if (_resourceManager->has<ecs::IInputProvider>()) {
        auto inputProvider = _resourceManager->get<ecs::IInputProvider>();
        if (inputProvider->isActionPressed(ecs::InputAction::MENU_BACK)) {
            if (auto stateMachine = _gsm.lock()) {
                stateMachine->requestStatePush(std::make_shared<PauseState>(stateMachine,
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

    renderHUD(deltaTime);
}

void InGameState::renderHUD(float deltaTime) {
    auto window = _resourceManager->get<gfx::IWindow>();
    auto currentCenter = window->getViewCenter();
    window->setViewCenter(constants::MAX_WIDTH / 2.0f, constants::MAX_HEIGHT / 2.0f);

    int score = 0;
    float health = 0.0f;
    float maxHealth = 0.0f;
    float shotCharge = 0.0f;
    float maxShotCharge = 0.0f;

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
        if (_registry->hasComponent<ecs::ChargedShotComponent>(entity)) {
            auto chargeComp = _registry->getComponent<ecs::ChargedShotComponent>(entity);
            shotCharge = (std::max)(chargeComp->getCharge(), 0.0f);
            maxShotCharge = chargeComp->getMaxCharge();
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

    drawHealthHUD(window, health, maxHealth);
    drawScoreHUD(window, score);
    drawShotChargeHUD(window, shotCharge, maxShotCharge);
    drawInGameMetrics(window, deltaTime);

    window->setViewCenter(currentCenter.getX(), currentCenter.getY());
}

void InGameState::drawHealthHUD(
    std::shared_ptr<gfx::IWindow> window,
    float health,
    float maxHealth
) {
    size_t barX = 20;
    size_t barY = static_cast<size_t>(constants::MAX_HEIGHT - 35);
    float barWidth = 200.0f;
    size_t barHeight = 20;
    size_t textOffsetY = 35;
    size_t feedbackBaseOffsetY = 105;

    std::stringstream healthSs;
    float displayMaxHealth = (health > 100.0f) ? health : maxHealth;
    healthSs << "Health: " << static_cast<int>(health) << "/" <<
        static_cast<int>(displayMaxHealth);
    std::string healthText = healthSs.str();

    float healthRatio = (maxHealth > 0.0f) ? health / maxHealth : 0.0f;
    if (healthRatio < 0.0f) healthRatio = 0.0f;
    if (healthRatio > 1.0f) healthRatio = 1.0f;
    uint8_t red = static_cast<uint8_t>((1.0f - healthRatio) * 255.0f);
    uint8_t green = static_cast<uint8_t>(healthRatio * 255.0f);
    gfx::color_t barColor = {red, green, 0, 255};
    std::pair<size_t, size_t> barPosition = {barX, barY};
    std::pair<size_t, size_t> barSize =
        {static_cast<size_t>(barWidth * healthRatio), barHeight};
    window->drawRoundedRectangleFilled(
        colors::BLACK, barPosition, {static_cast<size_t>(barWidth), barHeight}, 5.0f);
    window->drawRoundedRectangleFilled(
        barColor, barPosition, barSize, 5.0f);

    window->drawRoundedRectangleOutline(
        colors::WHITE, barPosition, {static_cast<size_t>(barWidth), barHeight}, 5.0f);
    std::pair<size_t, size_t> healthTextPosition =
        {barX, static_cast<size_t>(barY - textOffsetY)};
    window->drawText(healthText, colors::WHITE, healthTextPosition,
        constants::MAIN_FONT, 20, colors::BLACK, 1.0f);

    for (const auto& feedback : _healthFeedbacks) {
        uint8_t alpha =
            static_cast<uint8_t>((feedback.lifetime / feedback.maxLifetime) * 255.0f);
        gfx::color_t feedbackColor = {colors::RED.r, colors::RED.g, colors::RED.b, alpha};
        size_t x = static_cast<size_t>(barX);
        float base_y = constants::MAX_HEIGHT - static_cast<float>(feedbackBaseOffsetY);
        float progress = 1.0f - (feedback.lifetime / feedback.maxLifetime);
        size_t y = static_cast<size_t>(base_y - progress * 50.0f);
        std::pair<size_t, size_t> feedbackPosition = {x, y};
        window->drawText(
            feedback.text, feedbackColor, feedbackPosition,
            constants::MAIN_FONT, 28
        );
    }
}

void InGameState::drawScoreHUD(std::shared_ptr<gfx::IWindow> window, int score) {
    size_t barX = 250;
    size_t barY = static_cast<size_t>(constants::MAX_HEIGHT - 35);
    float barWidth = 150.0f;
    size_t barHeight = 20;
    size_t textOffsetY = 35;
    size_t feedbackBaseOffsetY = 105;

    std::pair<size_t, size_t> labelPosition = {barX, barY - textOffsetY};
    window->drawText("Score", colors::WHITE, labelPosition,
        constants::MAIN_FONT, 20, colors::BLACK, 1.0f);

    std::pair<size_t, size_t> rectPosition = {barX, barY};
    window->drawRoundedRectangleFilled(
        colors::BLACK, rectPosition, {static_cast<size_t>(barWidth), barHeight}, 5.0f);
    window->drawRoundedRectangleOutline(
        colors::WHITE, rectPosition, {static_cast<size_t>(barWidth), barHeight}, 5.0f);

    if (score < 0)
        score = 0;
    std::stringstream scoreSs;
    scoreSs << std::setfill('0') << std::setw(7) << score;
    std::string scoreText = scoreSs.str();
    std::pair<size_t, size_t> scorePosition = {barX + 10, barY - 2};
    window->drawText(
        scoreText, colors::YELLOW, scorePosition,
        constants::MAIN_FONT, 20
    );

    for (const auto& feedback : _scoreFeedbacks) {
        uint8_t alpha =
            static_cast<uint8_t>((feedback.lifetime / feedback.maxLifetime) * 255.0f);
        gfx::color_t feedbackColor =
            {colors::GREEN.r, colors::GREEN.g, colors::GREEN.b, alpha};
        size_t x = barX;
        size_t base_y = static_cast<size_t>(constants::MAX_HEIGHT) - feedbackBaseOffsetY;
        float progress = 1.0f - (feedback.lifetime / feedback.maxLifetime);
        size_t y = base_y - static_cast<size_t>(progress * 50.0f);
        std::pair<size_t, size_t> feedbackPosition = {x, y};
        window->drawText(
            feedback.text, feedbackColor, feedbackPosition,
            constants::MAIN_FONT, 28
        );
    }
}

void InGameState::drawShotChargeHUD(
    std::shared_ptr<gfx::IWindow> window,
    float shotCharge,
    float maxShotCharge
) {
    size_t barX = 430;
    size_t barY = static_cast<size_t>(constants::MAX_HEIGHT - 35);
    float barWidth = 200.0f;
    size_t barHeight = 20;
    size_t textOffsetY = 35;

    std::stringstream chargeSs;
    if (maxShotCharge <= 0.0f)
        maxShotCharge = 1.0f;
    chargeSs << "Charged Shot: " << static_cast<int>(
        shotCharge / (std::max)(0.0f, maxShotCharge) * 100
    ) << "%";
    std::string chargeText = chargeSs.str();

    float chargeRatio = (maxShotCharge > 0.0f) ? shotCharge / maxShotCharge : 0.0f;
    uint8_t red = static_cast<uint8_t>((1.0f - chargeRatio) * 255.0f);
    uint8_t green = static_cast<uint8_t>(chargeRatio * 255.0f);
    gfx::color_t barColor = {red, green, 0, 255};

    std::pair<size_t, size_t> barPosition = {barX, barY};

    std::pair<size_t, size_t> barSize = {
        static_cast<size_t>(barWidth * chargeRatio),
        barHeight
    };

    window->drawRoundedRectangleFilled(
        colors::BLACK, barPosition, {static_cast<size_t>(barWidth), barHeight}, 5.0f
    );
    window->drawRoundedRectangleFilled(
        barColor, barPosition, barSize, 5.0f
    );
    window->drawRoundedRectangleOutline(
        colors::WHITE, barPosition, {static_cast<size_t>(barWidth), barHeight}, 5.0f
    );

    std::pair<size_t, size_t> chargeTextPosition = {
        barX,
        static_cast<size_t>(barY - textOffsetY)
    };

    window->drawText(
        chargeText, colors::WHITE, chargeTextPosition,
        constants::MAIN_FONT, 20, colors::BLACK, 1.0f
    );
}

std::string getCPUUsage() {
    static clock_t lastCPU = 0;
    static auto lastTime = std::chrono::steady_clock::now();
    clock_t currentCPU = clock();
    auto currentTime = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(currentTime - lastTime).count();
    double cpuTime = static_cast<double>(currentCPU - lastCPU) / CLOCKS_PER_SEC;
    lastCPU = currentCPU;
    lastTime = currentTime;
    if (elapsed > 0) {
        double cpuPercent = (cpuTime / elapsed) * 100.0;
        if (cpuPercent > 100.0) cpuPercent = 100.0;
        return std::to_string(static_cast<int>(cpuPercent)) + "%";
    }
    return "0%";
}

std::string getMemoryUsage() {
#ifdef _WIN32
    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        SIZE_T memoryKB = pmc.WorkingSetSize / 1024;
        return std::to_string(memoryKB / 1024) + " MB";
    }
    return "N/A";
#else
    std::ifstream statm("/proc/self/statm");
    if (!statm.is_open())
        return "N/A";
    int64_t pages;
    statm >> pages;
    statm.close();
    int64_t pageSize = static_cast<int64_t>(SystemConfig::getPageSize());
    int64_t memoryKB = pages * pageSize / 1024;
    return std::to_string(memoryKB / 1024) + " MB";
#endif
}

void InGameState::drawInGameMetrics(std::shared_ptr<gfx::IWindow> window, float deltaTime) {
    auto config = _resourceManager->get<SettingsConfig>();
    if (!config->isInGameMetricsEnabled()) {
        return;
    }

    int fps = static_cast<int>(1.0f / deltaTime);

    math::Vector2f playerPosition(0.0f, 0.0f);
    math::Vector2f playerVelocity(0.0f, 0.0f);
    bool hasPlayerData = false;

    auto view = _registry->view<ecs::PlayerTag, ecs::LocalPlayerTag>();
    for (auto entity : view) {
        if (_registry->hasComponent<ecs::NetworkStateComponent>(entity)) {
            auto networkComp = _registry->getComponent<ecs::NetworkStateComponent>(entity);
            if (networkComp->hasTransform()) {
                playerPosition = networkComp->getCurrentTransform().position;
                hasPlayerData = true;
            }
        }
        if (_registry->hasComponent<ecs::VelocityComponent>(entity)) {
            auto velocityComp = _registry->getComponent<ecs::VelocityComponent>(entity);
            playerVelocity = velocityComp->getVelocity();
            hasPlayerData = true;
        }
        break;
    }

    std::string cpuUsage = getCPUUsage();
    std::string memoryUsage = getMemoryUsage();
    std::string metricsText = "";

    metricsText += std::to_string(fps) + " : FPS\n";
    if (hasPlayerData) {
        metricsText += std::to_string(
            static_cast<int>(playerVelocity.getX())) + ", " +
            std::to_string(static_cast<int>(playerVelocity.getY())) + " : Vel\n";
        metricsText += std::to_string(
            static_cast<int>(playerPosition.getX())) + ", " +
            std::to_string(static_cast<int>(playerPosition.getY())) + " : Pos\n";
    }
    metricsText += cpuUsage + " : CPU\n";
    metricsText += memoryUsage + " : Mem";

    auto textSize = window->getTextSize(metricsText, constants::MAIN_FONT, 12);
    size_t textHeight = textSize.second;
    size_t numLines = static_cast<size_t>(
        std::count(metricsText.begin(), metricsText.end(), '\n')) + 1;
    size_t totalHeight = textHeight + (numLines - 1) * 5;
    size_t baseX = static_cast<size_t>(constants::MAX_WIDTH - 5.0f);
    size_t baseY = static_cast<size_t>(constants::MAX_HEIGHT - static_cast<float>(totalHeight) - 15.0f);

    std::istringstream iss(metricsText);
    std::string line;
    size_t currentY = baseY;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            auto lineSize = window->getTextSize(line, constants::MAIN_FONT, 12);
            size_t lineWidth = lineSize.first;
            size_t lineX = baseX - lineWidth;
            std::pair<size_t, size_t> linePosition = {lineX, currentY};
            window->drawText(line, colors::WHITE, linePosition,
                constants::MAIN_FONT, 12, colors::BLACK, 1.0f);
            currentY += lineSize.second + 5;
        }
    }
}

void InGameState::exit() {
    _systems.clear();
    if (_registry) {
        _registry->clearAllEntities();
    }
}

}  // namespace gsm
