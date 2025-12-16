/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ResultsState
*/

#include "ResultsState.hpp"
#include <memory>
#include <string>
#include "../../../common/ECS/entity/registry/Registry.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "components/rendering/TextComponent.hpp"
#include "../../../common/constants.hpp"
#include "../../../libs/Multimedia/IEvent.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "../../../common/systems/systemManager/ISystemManager.hpp"
#include "../../../../systems/rendering/TextRenderingSystem.hpp"
#include "../../../../constants.hpp"

namespace gsm {

ResultsState::ResultsState(
    std::shared_ptr<IGameStateMachine> gsm,
    std::shared_ptr<ResourceManager> resourceManager,
    bool isWin)
    : AGameState(gsm, resourceManager), _isWin(isWin),
    _registry(resourceManager->get<ecs::Registry>()) {
}

void ResultsState::enter() {
    addSystem(std::make_shared<ecs::TextRenderingSystem>());

    ecs::Entity textEntity = _registry->createEntity();
    std::string text = _isWin ? constants::WIN_TEXT : constants::LOSE_TEXT;
    _registry->addComponent(textEntity, std::make_shared<ecs::TextComponent>(
        text, "assets/fonts/arial.ttf", gfx::color_t{255, 255, 255}));
    _registry->addComponent(textEntity,
        std::make_shared<ecs::TransformComponent>(
        math::Vector2f(constants::MAX_WIDTH / 2.0f - 50.0f,
        constants::MAX_HEIGHT / 2.0f), 0.0f,
        math::Vector2f(5.0f, 5.0f)));
}

void ResultsState::update(float deltaTime) {
    auto eventResult = _resourceManager->get<gfx::IEvent>()->pollEvents();
    if (eventResult == gfx::EventType::CLOSE) {
        _resourceManager->get<gfx::IWindow>()->closeWindow();
        return;
    }

    _resourceManager->get<ecs::ISystemManager>()->updateAllSystems
        (_resourceManager, _registry, deltaTime);
}

void ResultsState::exit() {
    // TODO(anyone): Cleanup results state
}

}  // namespace gsm
