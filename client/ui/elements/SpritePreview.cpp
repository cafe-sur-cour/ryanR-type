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
                constants::ANIMATIONCOMPONENT)) {
            if (extractAnimationFromPrefab(prefab)) {
                _type = PreviewType::Animation;
                _loaded = true;
                return true;
            }
        }

        if (prefab.contains(constants::PREFAB_COMPONENTS_FIELD) &&
            prefab[constants::PREFAB_COMPONENTS_FIELD].contains(
                constants::SPRITECOMPONENT)) {
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

        if (!components.contains(constants::SPRITECOMPONENT)) {
            return false;
        }

        const auto& spriteComp = components[constants::SPRITECOMPONENT];

        if (!spriteComp.contains(constants::FILEPATH_FIELD)) {
            return false;
        }

        _spriteData.texturePath =
            spriteComp[constants::FILEPATH_FIELD].get<std::string>();

        float frameWidth = 0.0f;
        float frameHeight = 0.0f;

        if (components.contains(constants::COLLIDERCOMPONENT)) {
            const auto& collider = components[constants::COLLIDERCOMPONENT];
            if (collider.contains(constants::SIZE_FIELD)) {
                frameWidth = collider[constants::SIZE_FIELD].value(
                    constants::X_FIELD, 0.0f);
                frameHeight = collider[constants::SIZE_FIELD].value(
                    constants::Y_FIELD, 0.0f);
            }
        }

        _spriteData.frameSize = math::Vector2f(frameWidth, frameHeight);

        if (components.contains(constants::TRANSFORMCOMPONENT)) {
            const auto& transform = components[constants::TRANSFORMCOMPONENT];

            if (transform.contains(constants::SCALE_FIELD)) {
                _spriteData.scale = math::Vector2f(
                    transform[constants::SCALE_FIELD].value(
                        constants::X_FIELD, 1.0f),
                    transform[constants::SCALE_FIELD].value(
                        constants::Y_FIELD, 1.0f)
                );
            } else {
                _spriteData.scale = math::Vector2f(1.0f, 1.0f);
            }

            _spriteData.rotation = transform.value(constants::ROTATION_FIELD, 0.0f);
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

        if (!components.contains(constants::ANIMATIONCOMPONENT)) {
            return false;
        }

        const auto& animComp = components[constants::ANIMATIONCOMPONENT];

        if (!animComp.contains(constants::STATES_FIELD)) {
            return false;
        }

        const auto& states = animComp[constants::STATES_FIELD];
        if (states.empty()) {
            return false;
        }

        auto it = states.begin();
        const auto& firstState = it.value();

        if (!firstState.contains(constants::TEXTUREPATH_FIELD)) {
            return false;
        }

        _animationData.texturePath =
            firstState[constants::TEXTUREPATH_FIELD].get<std::string>();
        _animationData.frameSize = math::Vector2f(
            firstState.value(constants::FRAMEWIDTH_FIELD, 0.0f),
            firstState.value(constants::FRAMEHEIGHT_FIELD, 0.0f)
        );
        _animationData.frameCount =
            firstState.value(constants::FRAMECOUNT_FIELD, 1.0f);
        _animationData.startWidth =
            firstState.value(constants::STARTWIDTH_FIELD, 0.0f);
        _animationData.startHeight =
            firstState.value(constants::STARTHEIGHT_FIELD, 0.0f);
        _animationData.speed = firstState.value(constants::SPEED_FIELD, 0.1f);
        _animationData.loop = firstState.value(constants::LOOP_FIELD, true);

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

void SpritePreview::clear() {
    _type = PreviewType::None;
    _loaded = false;
    _currentFrame = 0.0f;
    _animationTime = 0.0f;
}

}  // namespace ui
