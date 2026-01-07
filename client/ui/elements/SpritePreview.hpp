/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SpritePreview
*/

#ifndef SPRITEPREVIEW_HPP_
#define SPRITEPREVIEW_HPP_

#include <memory>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "base/UIElement.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/resourceManager/ResourceManager.hpp"

namespace ui {

class SpritePreview : public UIElement {
    public:
        SpritePreview(std::shared_ptr<ResourceManager> resourceManager);
        ~SpritePreview() = default;

        bool loadPrefab(const std::filesystem::path& prefabPath);

        void setDisplayBounds(const math::Vector2f& position, const math::Vector2f& size);
        void setTransform(float scale, float rotation);

        void render() override;
        void update(float deltaTime) override;

    private:
        struct SpriteData {
            std::string texturePath;
            math::Vector2f frameSize;
            math::Vector2f position;
            math::Vector2f scale;
            float rotation = 0.0f;
        };

        struct AnimationData {
            std::string texturePath;
            math::Vector2f frameSize;
            float frameCount = 0.0f;
            float startWidth = 0.0f;
            float startHeight = 0.0f;
            float speed = 0.1f;
            bool loop = true;
        };

        enum class PreviewType {
            None,
            Sprite,
            Animation
        };

        PreviewType _type = PreviewType::None;
        SpriteData _spriteData;
        AnimationData _animationData;
        float _currentFrame = 0.0f;
        float _animationTime = 0.0f;
        bool _loaded = false;

        bool extractSpriteFromPrefab(const nlohmann::json& prefab);
        bool extractAnimationFromPrefab(const nlohmann::json& prefab);

        void renderSprite();
        void renderAnimation();
};

}  // namespace ui

#endif /* !SPRITEPREVIEW_HPP_ */
