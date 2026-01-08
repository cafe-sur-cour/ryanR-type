/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpritePreview
*/

#include "SpritePreview.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include "../../../common/interfaces/IWindow.hpp"
#include "../../constants.hpp"

namespace ui {

SpritePreview::SpritePreview(std::shared_ptr<ResourceManager> resourceManager)
    : UIElement(resourceManager) {
}

bool SpritePreview::loadPrefab(const std::filesystem::path& prefabPath) {
    try {
        std::ifstream file(prefabPath);
        if (!file.is_open()) {
            return false;
        }

        nlohmann::json prefab;
        file >> prefab;
        file.close();

        if (prefab.contains(constants::PREFAB_COMPONENTS_FIELD) &&
            prefab[constants::PREFAB_COMPONENTS_FIELD].contains(
                constants::PREFAB_ANIMATION_COMPONENT)) {
            if (extractAnimationFromPrefab(prefab)) {
                _type = PreviewType::Animation;
                _loaded = true;
                return true;
            }
        }

        if (prefab.contains(constants::PREFAB_COMPONENTS_FIELD) &&
            prefab[constants::PREFAB_COMPONENTS_FIELD].contains(
                constants::PREFAB_SPRITE_COMPONENT)) {
            if (extractSpriteFromPrefab(prefab)) {
                _type = PreviewType::Sprite;
                _loaded = true;
                return true;
            }
        }

        return false;
    } catch (const std::exception&) {
        return false;
    }
}

bool SpritePreview::extractSpriteFromPrefab(const nlohmann::json& prefab) {
    try {
        const auto& components = prefab[constants::PREFAB_COMPONENTS_FIELD];

        if (!components.contains(constants::PREFAB_SPRITE_COMPONENT)) {
            return false;
        }

        const auto& spriteComp = components[constants::PREFAB_SPRITE_COMPONENT];

        if (!spriteComp.contains(constants::PREFAB_FILEPATH_FIELD)) {
            return false;
        }

        _spriteData.texturePath =
            spriteComp[constants::PREFAB_FILEPATH_FIELD].get<std::string>();

        float frameWidth = 0.0f;
        float frameHeight = 0.0f;

        if (components.contains(constants::PREFAB_COLLIDER_COMPONENT)) {
            const auto& collider = components[constants::PREFAB_COLLIDER_COMPONENT];
            if (collider.contains(constants::PREFAB_SIZE_FIELD)) {
                frameWidth = collider[constants::PREFAB_SIZE_FIELD].value(
                    constants::PREFAB_X_FIELD, 0.0f);
                frameHeight = collider[constants::PREFAB_SIZE_FIELD].value(
                    constants::PREFAB_Y_FIELD, 0.0f);
            }
        }

        _spriteData.frameSize = math::Vector2f(frameWidth, frameHeight);

        if (components.contains(constants::PREFAB_TRANSFORM_COMPONENT)) {
            const auto& transform = components[constants::PREFAB_TRANSFORM_COMPONENT];

            if (transform.contains(constants::PREFAB_SCALE_FIELD)) {
                _spriteData.scale = math::Vector2f(
                    transform[constants::PREFAB_SCALE_FIELD].value(
                        constants::PREFAB_X_FIELD, 1.0f),
                    transform[constants::PREFAB_SCALE_FIELD].value(
                        constants::PREFAB_Y_FIELD, 1.0f)
                );
            } else {
                _spriteData.scale = math::Vector2f(1.0f, 1.0f);
            }

            _spriteData.rotation = transform.value(constants::PREFAB_ROTATION_FIELD, 0.0f);
        } else {
            _spriteData.scale = math::Vector2f(1.0f, 1.0f);
        }

        return true;
    } catch (const std::exception&) {
        return false;
    }
}

bool SpritePreview::extractAnimationFromPrefab(const nlohmann::json& prefab) {
    try {
        const auto& components = prefab[constants::PREFAB_COMPONENTS_FIELD];

        if (!components.contains(constants::PREFAB_ANIMATION_COMPONENT)) {
            return false;
        }

        const auto& animComp = components[constants::PREFAB_ANIMATION_COMPONENT];

        if (!animComp.contains(constants::PREFAB_STATES_FIELD)) {
            return false;
        }

        const auto& states = animComp[constants::PREFAB_STATES_FIELD];
        if (states.empty()) {
            return false;
        }

        auto it = states.begin();
        const auto& firstState = it.value();

        if (!firstState.contains(constants::PREFAB_TEXTURE_PATH_FIELD)) {
            return false;
        }

        _animationData.texturePath =
            firstState[constants::PREFAB_TEXTURE_PATH_FIELD].get<std::string>();
        _animationData.frameSize = math::Vector2f(
            firstState.value(constants::PREFAB_FRAME_WIDTH_FIELD, 0.0f),
            firstState.value(constants::PREFAB_FRAME_HEIGHT_FIELD, 0.0f)
        );
        _animationData.frameCount =
            firstState.value(constants::PREFAB_FRAME_COUNT_FIELD, 1.0f);
        _animationData.startWidth =
            firstState.value(constants::PREFAB_START_WIDTH_FIELD, 0.0f);
        _animationData.startHeight =
            firstState.value(constants::PREFAB_START_HEIGHT_FIELD, 0.0f);
        _animationData.speed = firstState.value(constants::PREFAB_SPEED_FIELD, 0.1f);
        _animationData.loop = firstState.value(constants::PREFAB_LOOP_FIELD, true);

        _currentFrame = 0.0f;
        _animationTime = 0.0f;

        return true;
    } catch (const std::exception&) {
        return false;
    }
}

void SpritePreview::setDisplayBounds(
    const math::Vector2f& position,
    const math::Vector2f& size
) {
    setPosition(position);
    setSize(size);
}

void SpritePreview::setTransform(float scale, float rotation) {
    if (_type == PreviewType::Sprite) {
        _spriteData.scale = math::Vector2f(scale, scale);
        _spriteData.rotation = rotation;
    }
}

void SpritePreview::render() {
    if (!_loaded || !_visible) {
        return;
    }

    if (_type == PreviewType::Sprite) {
        renderSprite();
    } else if (_type == PreviewType::Animation) {
        renderAnimation();
    }
}

void SpritePreview::renderSprite() {
    try {
        auto resourceMgr = _resourceManager.lock();
        if (!resourceMgr) return;

        auto window = resourceMgr->get<gfx::IWindow>();
        if (!window) return;

        math::Vector2f absPos = getAbsolutePosition();
        math::Vector2f absSize = getAbsoluteSize();

        float scaleX = absSize.getX() / _spriteData.frameSize.getX();
        float scaleY = absSize.getY() / _spriteData.frameSize.getY();

        window->drawSprite(
            _spriteData.texturePath,
            absPos.getX(),
            absPos.getY(),
            scaleX,
            scaleY
        );
    } catch (const std::exception&) {
    }
}

void SpritePreview::renderAnimation() {
    try {
        auto resourceMgr = _resourceManager.lock();
        if (!resourceMgr) return;

        auto window = resourceMgr->get<gfx::IWindow>();
        if (!window) return;

        math::Vector2f absPos = getAbsolutePosition();
        math::Vector2f absSize = getAbsoluteSize();

        float scaleX = absSize.getX() / _animationData.frameSize.getX();
        float scaleY = absSize.getY() / _animationData.frameSize.getY();

        int currentFrameIndex = static_cast<int>(_currentFrame);
        float frameX = currentFrameIndex * _animationData.frameSize.getX();
        float frameY = 0.0f;
        math::FRect frameRect(
            frameX, frameY, _animationData.frameSize.getX(), _animationData.frameSize.getY());

        window->drawSprite(
            _animationData.texturePath,
            absPos.getX(),
            absPos.getY(),
            frameRect,
            scaleX,
            scaleY
        );
    } catch (const std::exception&) {
    }
}

void SpritePreview::update(float deltaTime) {
    if (!_loaded || _type != PreviewType::Animation) {
        return;
    }

    _animationTime += deltaTime;
    float frameDuration = _animationData.speed;

    if (_animationTime >= frameDuration) {
        _animationTime = 0.0f;
        _currentFrame += 1.0f;

        if (_currentFrame >= _animationData.frameCount) {
            if (_animationData.loop) {
                _currentFrame = 0.0f;
            } else {
                _currentFrame = _animationData.frameCount - 1.0f;
            }
        }
    }
}

}  // namespace ui
