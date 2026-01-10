/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LevelEditorRender
*/

#include <sstream>
#include <algorithm>
#include <utility>
#include <string>
#include "LevelEditorState.hpp"
#include "../../../../../common/interfaces/IEvent.hpp"
#include "../../../../constants.hpp"
#include "../../../../colors.hpp"

namespace gsm {

void LevelEditorState::renderUI() {
    _uiManager->render();
    renderLevelPreview();
}

void LevelEditorState::handleZoom(
    [[maybe_unused]] float deltaTime,
    gfx::EventType eventResult
) {
    auto event = _resourceManager->get<gfx::IEvent>();

    bool zoomIn = event->isKeyPressed(gfx::EventType::NUMPAD_ADD);
    bool zoomOut = event->isKeyPressed(gfx::EventType::NUMPAD_SUBTRACT);

    bool wheelUp = eventResult == gfx::EventType::MOUSEWHEELUP;
    bool wheelDown = eventResult == gfx::EventType::MOUSEWHEELDOWN;

    if (zoomIn || zoomOut || wheelUp || wheelDown) {
        const float sidePanelWidth = 300.0f;
        const float bottomPanelHeight = 200.0f;
        const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

        math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();

        bool isInCanvas = mousePos.getX() >= sidePanelWidth &&
                          mousePos.getX() <= constants::MAX_WIDTH &&
                          mousePos.getY() >= 0.0f &&
                          mousePos.getY() <= canvasHeight;

        float cursorLevelX =
            _viewportOffset.getX() + (mousePos.getX() - sidePanelWidth) / _viewportZoom;
        float cursorLevelY = _viewportOffset.getY() + mousePos.getY() / _viewportZoom;

        float zoomFactor = (zoomIn || wheelUp) ? 1.1f : 0.9f;
        float oldZoom = _viewportZoom;
        float newZoom = _viewportZoom * zoomFactor;

        if (newZoom > _maxZoom)
            newZoom = _maxZoom;

        if (newZoom < _minZoom)
            newZoom = _minZoom;

        _viewportZoom = newZoom;

        if (isInCanvas && oldZoom != 0.0f) {
            float newOffsetX = cursorLevelX - (mousePos.getX() - sidePanelWidth) / newZoom;
            float newOffsetY = cursorLevelY - mousePos.getY() / newZoom;
            _viewportOffset.setX(newOffsetX);
            _viewportOffset.setY(newOffsetY);
        }
    }
}

void LevelEditorState::handleCanvasDrag([[maybe_unused]] float deltaTime) {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    math::Vector2f mousePos = _mouseHandler->getWorldMousePosition();
    bool rightMousePressed = _mouseHandler->isMouseButtonPressed(
        static_cast<int>(constants::MouseButton::RIGHT));

    bool isInCanvas = mousePos.getX() >= sidePanelWidth &&
                      mousePos.getX() <= constants::MAX_WIDTH &&
                      mousePos.getY() >= 0.0f &&
                      mousePos.getY() <= canvasHeight;

    if (rightMousePressed && isInCanvas) {
        if (!_isDragging) {
            _isDragging = true;
            _dragStartPos = mousePos;
            _lastMousePos = mousePos;
        } else {
            math::Vector2f mouseDelta = mousePos - _lastMousePos;

            _viewportOffset.setX(_viewportOffset.getX() - mouseDelta.getX() / _viewportZoom);
            _viewportOffset.setY(_viewportOffset.getY() - mouseDelta.getY() / _viewportZoom);
            _lastMousePos = mousePos;
        }
    } else {
        _isDragging = false;
    }
}

void LevelEditorState::renderLevelPreview() {
    const float sidePanelWidth = 300.0f;
    const float bottomPanelHeight = 200.0f;
    const float canvasHeight = constants::MAX_HEIGHT - bottomPanelHeight;

    float mapLength = _levelData.value(constants::MAP_LENGTH_FIELD, 0.0f);

    if (mapLength <= 0.0f) {
        return;
    }

    float levelWidth = mapLength * _viewportZoom;
    float levelHeight = constants::MAX_HEIGHT * _viewportZoom;
    float levelX = sidePanelWidth - (_viewportOffset.getX() * _viewportZoom);
    float levelY = -(_viewportOffset.getY() * _viewportZoom);

    const float canvasLeft = sidePanelWidth;
    const float canvasRight = constants::MAX_WIDTH;
    const float canvasTop = 0.0f;
    const float canvasBottom = canvasHeight;

    if (levelX + levelWidth < canvasLeft || levelX > canvasRight ||
        levelY + levelHeight < canvasTop || levelY > canvasBottom) {
        return;
    }

    auto window = _resourceManager->get<gfx::IWindow>();
    gfx::color_t red = colors::RED;

    const float borderThickness = 2.0f;

    if (levelY >= canvasTop && levelY < canvasBottom) {
        float topX = (std::max)(levelX, canvasLeft);
        float topXEnd = (std::min)(levelX + levelWidth, canvasRight);
        float topWidth = topXEnd - topX;
        if (topWidth > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(topX), static_cast<size_t>(levelY)),
                std::make_pair(static_cast<size_t>(topWidth),
                    static_cast<size_t>(borderThickness))
            );
        }
    }

    float bottomY = levelY + levelHeight - borderThickness;
    if (bottomY >= canvasTop && bottomY < canvasBottom) {
        float bottomX = (std::max)(levelX, canvasLeft);
        float bottomXEnd = (std::min)(levelX + levelWidth, canvasRight);
        float bottomWidth = bottomXEnd - bottomX;
        if (bottomWidth > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(bottomX), static_cast<size_t>(bottomY)),
                std::make_pair(static_cast<size_t>(bottomWidth),
                    static_cast<size_t>(borderThickness))
            );
        }
    }

    if (levelX >= canvasLeft && levelX < canvasRight) {
        float leftY = (std::max)(levelY, canvasTop);
        float leftYEnd = (std::min)(levelY + levelHeight, canvasBottom);
        float leftHeight = leftYEnd - leftY;
        if (leftHeight > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(levelX), static_cast<size_t>(leftY)),
                std::make_pair(static_cast<size_t>(borderThickness),
                    static_cast<size_t>(leftHeight))
            );
        }
    }

    float rightX = levelX + levelWidth - borderThickness;
    if (rightX >= canvasLeft && rightX < canvasRight) {
        float rightY = (std::max)(levelY, canvasTop);
        float rightYEnd = (std::min)(levelY + levelHeight, canvasBottom);
        float rightHeight = rightYEnd - rightY;
        if (rightHeight > 0) {
            window->drawFilledRectangle(
                red,
                std::make_pair(static_cast<size_t>(rightX), static_cast<size_t>(rightY)),
                std::make_pair(static_cast<size_t>(borderThickness),
                    static_cast<size_t>(rightHeight))
            );
        }
    }

    if (_displayFilter == "All" || _displayFilter == "Obstacles") {
        renderAllObstacles(levelX, levelY, canvasLeft, canvasRight, canvasTop, canvasBottom);
    }
    if (_displayFilter == "All" || _displayFilter == "PowerUps") {
        renderAllPowerUps(levelX, levelY, canvasLeft, canvasRight, canvasTop, canvasBottom);
    }
    if (_displayFilter == "All" || _displayFilter == "Waves") {
        renderAllWaves(levelX, levelY, canvasLeft, canvasRight, canvasTop, canvasBottom);
    }
}

void LevelEditorState::renderSpriteInLevelPreview(
    const LevelPreviewSprite& spriteData,
    const std::string& prefabName,
    float screenX,
    float screenY,
    float canvasLeft,
    float canvasRight,
    float canvasTop,
    float canvasBottom
) {
    float scaledWidth = spriteData.width * _viewportZoom;
    float scaledHeight = spriteData.height * _viewportZoom;

    if (screenX + scaledWidth <= canvasLeft || screenX >= canvasRight ||
        screenY + scaledHeight <= canvasTop || screenY >= canvasBottom) {
        return;
    }

    if (screenX >= canvasLeft && screenX + scaledWidth <= canvasRight &&
        screenY >= canvasTop && screenY + scaledHeight <= canvasBottom) {
        auto window = _resourceManager->get<gfx::IWindow>();
        float finalScaleX = _viewportZoom * spriteData.scale;
        float finalScaleY = _viewportZoom * spriteData.scale;

        if (spriteData.isAnimation && spriteData.frameCount > 0.0f) {
            float currentFrameFloat = 0.0f;
            if (_obstacleAnimationFrames.find(prefabName) != _obstacleAnimationFrames.end()) {
                currentFrameFloat = _obstacleAnimationFrames[prefabName];
            } else if (_powerUpAnimationFrames.find(prefabName) !=
                _powerUpAnimationFrames.end()) {
                currentFrameFloat = _powerUpAnimationFrames[prefabName];
            }
            int currentFrameIndex = static_cast<int>(currentFrameFloat);
            float frameX = static_cast<float>(currentFrameIndex) * spriteData.frameWidth;
            float frameY = 0.0f;
            math::FRect frameRect(
                frameX, frameY, spriteData.frameWidth, spriteData.frameHeight);

            window->drawSprite(
                spriteData.texturePath,
                screenX,
                screenY,
                frameRect,
                finalScaleX,
                finalScaleY,
                spriteData.rotation
            );
        } else {
            window->drawSprite(
                spriteData.texturePath,
                screenX,
                screenY,
                finalScaleX,
                finalScaleY,
                spriteData.rotation
            );
        }

        if (_showHitboxes) {
            gfx::color_t green = colors::GREEN;
            window->drawRectangleOutline(
                green,
                std::make_pair(static_cast<size_t>(screenX),
                    static_cast<size_t>(screenY)),
                std::make_pair(static_cast<size_t>(scaledWidth),
                    static_cast<size_t>(scaledHeight)),
                1
            );
        }
    }
}

}  // namespace gsm
