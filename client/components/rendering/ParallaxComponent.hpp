/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ParallaxComponent
*/

#ifndef PARALLAXCOMPONENT_HPP_
#define PARALLAXCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../common/constants.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <memory>

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

class ParallaxComponent : public IComponent {
    public:
        ParallaxComponent()
            : _baseScrollSpeed(1.0f),
              _direction(constants::BACKGROUND_PARALLAX_DIRECTION_X, constants::BACKGROUND_PARALLAX_DIRECTION_Y),
              _layers() {}

        ~ParallaxComponent() = default;

        float getBaseScrollSpeed() const { return _baseScrollSpeed; }
        const math::Vector2f& getDirection() const { return _direction; }
        std::vector<std::shared_ptr<ParallaxLayer>> &getLayers() { return _layers; }

        void setBaseScrollSpeed(float speed) { _baseScrollSpeed = speed; }
        void setDirection(const math::Vector2f& direction) { _direction = direction; }
        void addLayer(std::shared_ptr<ParallaxLayer> layer) { _layers.push_back(layer); }

    private:
        float _baseScrollSpeed;
        math::Vector2f _direction;
        std::vector<std::shared_ptr<ParallaxLayer>> _layers;
};

}  // namespace ecs

#endif /* !PARALLAXCOMPONENT_HPP_ */
