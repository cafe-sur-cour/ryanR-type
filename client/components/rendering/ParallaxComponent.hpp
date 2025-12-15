/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParallaxComponent
*/

#ifndef PARALLAXCOMPONENT_HPP_
#define PARALLAXCOMPONENT_HPP_

#include "../../../common/components/base/AComponent.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/constants.hpp"
#include <string>
#include <vector>

namespace ecs {

enum class ParallaxScaleMode {
    FIT_SCREEN = 0,
    STRETCH = 1,
    MANUAL = 2,
};

struct ParallaxLayer {
    std::string name;
    std::string filePath;
    float speedMultiplier;
    math::Vector2f scale;
    ParallaxScaleMode scaleMode;
    math::Vector2f sourceSize;
    bool repeat;
    int zIndex;
    math::Vector2f currentOffset;

    ParallaxLayer()
        : name(""),
          filePath(""),
          speedMultiplier(1.0f),
          scale(1.0f, 1.0f),
          scaleMode(ParallaxScaleMode::FIT_SCREEN),
          sourceSize(constants::DEFAULT_TEXTURE_WIDTH, constants::DEFAULT_TEXTURE_HEIGHT),
          repeat(true),
          zIndex(0),
          currentOffset(0.0f, 0.0f) {}
};

class ParallaxComponent : public AComponent {
    public:
        ParallaxComponent()
            : _baseScrollSpeed(1.0f),
              _direction(constants::BACKGROUND_PARALLAX_DIRECTION_X, constants::BACKGROUND_PARALLAX_DIRECTION_Y),
              _layers() {}

        ~ParallaxComponent() = default;

        float getBaseScrollSpeed() const { return _baseScrollSpeed; }
        const math::Vector2f& getDirection() const { return _direction; }
        const std::vector<ParallaxLayer>& getLayers() const { return _layers; }

        void setBaseScrollSpeed(float speed) { _baseScrollSpeed = speed; }
        void setDirection(const math::Vector2f& direction) { _direction = direction; }
        void addLayer(const ParallaxLayer& layer) { _layers.push_back(layer); }
        void clearLayers() { _layers.clear(); }

        void updateLayerOffsets(const math::Vector2f& direction, float baseSpeed, float deltaTime) {
            for (auto& layer : _layers) {
                float speed = baseSpeed * layer.speedMultiplier;
                math::Vector2f movement(
                    direction.getX() * speed * deltaTime,
                    direction.getY() * speed * deltaTime
                );
                layer.currentOffset = math::Vector2f(
                    layer.currentOffset.getX() + movement.getX(),
                    layer.currentOffset.getY() + movement.getY()
                );
            }
        }

        size_t getLayerCount() const { return _layers.size(); }

        void sortLayersByZIndex() {
            std::sort(_layers.begin(), _layers.end(),
                [](const ParallaxLayer& a, const ParallaxLayer& b) {
                    return a.zIndex < b.zIndex;
                });
        }

    private:
        float _baseScrollSpeed;
        math::Vector2f _direction;
        std::vector<ParallaxLayer> _layers;
};

}  // namespace ecs

#endif /* !PARALLAXCOMPONENT_HPP_ */
