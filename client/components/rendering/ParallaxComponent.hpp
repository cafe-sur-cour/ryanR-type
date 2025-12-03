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
#include <string>
#include <vector>

namespace ecs {

enum class ParallaxScaleMode {
    FIT_SCREEN,     // Adapt the texture to fit the screen while maintaining aspect ratio
    STRETCH,        // Stretch the texture to fill the screen (may distort aspect ratio)
    MANUAL          // Use only the provided scale values
};

struct ParallaxLayer {
    std::string name;
    std::string filePath;
    float speedMultiplier;
    math::Vector2f offset;
    math::Vector2f scale;
    ParallaxScaleMode scaleMode;
    math::Vector2f sourceSize;
    bool repeat;
    float opacity;
    int zIndex;
    math::Vector2f currentOffset;

    ParallaxLayer()
        : name(""),
          filePath(""),
          speedMultiplier(1.0f),
          offset(0.0f, 0.0f),
          scale(1.0f, 1.0f),
          scaleMode(ParallaxScaleMode::FIT_SCREEN),
          sourceSize(1920.0f, 1080.0f),
          repeat(true),
          opacity(1.0f),
          zIndex(0),
          currentOffset(0.0f, 0.0f) {}
};

class ParallaxComponent : public AComponent {
    public:
        ParallaxComponent()
            : _baseScrollSpeed(1.0f),
              _direction(-1.0f, 0.0f),
              _layers() {}

        ~ParallaxComponent() = default;

        // Getters
        float getBaseScrollSpeed() const { return _baseScrollSpeed; }
        const math::Vector2f& getDirection() const { return _direction; }
        const std::vector<ParallaxLayer>& getLayers() const { return _layers; }
        std::vector<ParallaxLayer>& getLayers() { return _layers; }

        // Setters
        void setBaseScrollSpeed(float speed) { _baseScrollSpeed = speed; }
        void setDirection(const math::Vector2f& direction) { _direction = direction; }
        void addLayer(const ParallaxLayer& layer) { _layers.push_back(layer); }
        void clearLayers() { _layers.clear(); }

        // Utility
        size_t getLayerCount() const { return _layers.size(); }
        ParallaxLayer* getLayer(size_t index) {
            if (index < _layers.size())
                return &_layers[index];
            return nullptr;
        }

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
