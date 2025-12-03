/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParallaxRenderingSystem
*/

#include "ParallaxRenderingSystem.hpp"
#include "../../components/rendering/ParallaxComponent.hpp"
#include "../../../common/components/permanent/TransformComponent.hpp"
#include "../../../common/ECS/view/View.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include <memory>
#include <cmath>
#include <iostream>

namespace ecs {

ParallaxRenderingSystem::ParallaxRenderingSystem() {
}

void ParallaxRenderingSystem::update(std::shared_ptr<ResourceManager> resourceManager,
    std::shared_ptr<Registry> registry, float deltaTime) {

    if (!resourceManager->has<gfx::IWindow>())
        return;

    auto window = resourceManager->get<gfx::IWindow>();
    auto windowSize = window->getWindowSize();
    float screenWidth = static_cast<float>(windowSize.first);
    float screenHeight = static_cast<float>(windowSize.second);

    View<ParallaxComponent, TransformComponent> view(registry);

    for (auto entityId : view) {
        auto parallax = registry->getComponent<ParallaxComponent>(entityId);
        auto transform = registry->getComponent<TransformComponent>(entityId);

        if (!parallax || !transform)
            continue;

        auto& layers = parallax->getLayers();
        const math::Vector2f& direction = parallax->getDirection();
        float baseSpeed = parallax->getBaseScrollSpeed();
        const math::Vector2f& basePosition = transform->getPosition();

        parallax->updateLayerOffsets(direction, baseSpeed, deltaTime);

        for (const auto& layer : layers) {
            renderLayer(layer, resourceManager, basePosition, screenWidth, screenHeight);
        }
    }
}

math::Vector2f ParallaxRenderingSystem::calculateScale(const ParallaxLayer& layer,
    float screenWidth, float screenHeight) {

    math::Vector2f finalScale = layer.scale;

    switch (layer.scaleMode) {
        case ParallaxScaleMode::FIT_SCREEN: {
            float scaleX = screenWidth / layer.sourceSize.getX();
            float scaleY = screenHeight / layer.sourceSize.getY();
            float uniformScale = std::max(scaleX, scaleY);
            finalScale = math::Vector2f(uniformScale, uniformScale);
            break;
        }
        case ParallaxScaleMode::STRETCH: {
            float scaleX = screenWidth / layer.sourceSize.getX();
            float scaleY = screenHeight / layer.sourceSize.getY();
            finalScale = math::Vector2f(scaleX, scaleY);
            break;
        }
        case ParallaxScaleMode::MANUAL:
            finalScale = layer.scale;
            break;
    }

    return finalScale;
}

void ParallaxRenderingSystem::renderLayer(const ParallaxLayer& layer,
    std::shared_ptr<ResourceManager> resourceManager,
    const math::Vector2f& basePosition, float screenWidth, float screenHeight) {

    if (!resourceManager->has<gfx::IWindow>())
        return;

    auto window = resourceManager->get<gfx::IWindow>();
    math::Vector2f scale = calculateScale(layer, screenWidth, screenHeight);

    float textureWidth = layer.sourceSize.getX() * scale.getX();
    float textureHeight = layer.sourceSize.getY() * scale.getY();

    if (layer.repeat) {
        float startX = layer.currentOffset.getX() + basePosition.getX();
        float startY = layer.currentOffset.getY() + basePosition.getY();

        startX = std::fmod(startX, textureWidth);
        if (startX > 0) startX -= textureWidth;

        startY = std::fmod(startY, textureHeight);
        if (startY > 0) startY -= textureHeight;

        int tilesX = static_cast<int>(std::ceil((screenWidth - startX) / textureWidth)) + 1;
        int tilesY = static_cast<int>(std::ceil((screenHeight - startY) / textureHeight)) + 1;

        for (int y = 0; y < tilesY; ++y) {
            for (int x = 0; x < tilesX; ++x) {
                float drawX = startX + (static_cast<float>(x) * textureWidth);
                float drawY = startY + (static_cast<float>(y) * textureHeight);

                window->drawSprite(layer.filePath, drawX, drawY, scale.getX(), scale.getY());
            }
        }
    } else {
        float drawX = layer.currentOffset.getX() + basePosition.getX();
        float drawY = layer.currentOffset.getY() + basePosition.getY();

        window->drawSprite(layer.filePath, drawX, drawY, scale.getX(), scale.getY());
    }
}

} // namespace ecs
