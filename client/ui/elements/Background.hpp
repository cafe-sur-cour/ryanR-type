/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Background
*/

#ifndef BACKGROUND_HPP_
#define BACKGROUND_HPP_

#include "base/UIElement.hpp"
#include "../../../common/constants.hpp"
#include <string>
#include <vector>

namespace ui {

class Background : public UIElement {
public:
    Background(std::shared_ptr<ResourceManager> resourceManager);
    ~Background() override = default;

    void render() override;
    void update(float deltaTime) override;

    void addLayer(const std::string& texturePath, float speedX, float speedY = 0.0f,
        const math::Vector2f& sourceSize = math::Vector2f(constants::MAX_WIDTH, constants::MAX_HEIGHT));

private:
    struct Layer {
        std::string texturePath;
        float speedX;
        float speedY;
        math::Vector2f sourceSize;
        float offsetX = 0.0f;
        float offsetY = 0.0f;
    };

    float calculateScale(const Layer& layer, float screenWidth);

    std::vector<Layer> _layers;
};

}  // namespace ui

#endif  // BACKGROUND_HPP_
