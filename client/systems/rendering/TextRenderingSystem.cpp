/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextRenderingSystem
*/

#include <memory>
#include "TextRenderingSystem.hpp"
#include "../../components/rendering/TextComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include <iostream>
namespace ecs {

TextRenderingSystem::TextRenderingSystem() {
}

void TextRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<Registry> registry, float deltaTime) {
    (void)deltaTime;

    View<TextComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto textComp = registry->getComponent<TextComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!textComp || !transform)
            continue;
        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const math::Vector2f& pos = transform->getPosition();
            std::pair<size_t, size_t> position(static_cast<size_t>(pos.getX()), static_cast<size_t>(pos.getY()));
            window->drawText(textComp->getText(), textComp->getColor(), position, textComp->getFontPath());
        }
    }
}

}  // namespace ecs
