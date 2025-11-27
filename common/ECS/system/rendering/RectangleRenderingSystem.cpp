/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** RectangleRenderingSystem
*/

#include <memory>
#include "RectangleRenderingSystem.hpp"
#include "../../component/rendering/RectangleRenderComponent.hpp"
#include "../../component/permanent/TransformComponent.hpp"
#include "../../view/View.hpp"
#include "../../resourceManager/ResourceManager.hpp"
#include "../../../../libs/Multimedia/IWindow.hpp"


namespace ecs {

RectangleRenderingSystem::RectangleRenderingSystem() {
}

void RectangleRenderingSystem::update(std::shared_ptr<ResourceManager>
    resourceManager, std::shared_ptr<ARegistry> registry, float deltaTime) {
    (void)deltaTime;

    View<RectangleRenderComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto rectangle =
            registry->getComponent<RectangleRenderComponent>(entityId);
        auto transform =
            registry->getComponent<TransformComponent>(entityId);

        if (!rectangle || !transform)
            continue;
        if (resourceManager->has<gfx::IWindow>()) {
            auto window = resourceManager->get<gfx::IWindow>();
            const math::Vector2f& pos = transform->getPosition();
            const gfx::color_t& color = rectangle->getColor();
            float width = rectangle->getWidth();
            float height = rectangle->getHeight();

            window->drawFilledRectangle(color,
                {static_cast<size_t>(pos.getX()),
                    static_cast<size_t>(pos.getY())},
                {static_cast<size_t>(width),
                    static_cast<size_t>(height)});
        }
    }
}

}  // namespace ecs
