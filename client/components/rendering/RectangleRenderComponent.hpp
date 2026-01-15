/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** RectangleRenderComponent
*/

#ifndef RECTANGLERENDERCOMPONENT_HPP_
#define RECTANGLERENDERCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include "../../../common/interfaces/IWindow.hpp"
#include "../../colors.hpp"

namespace ecs {

class RectangleRenderComponent : public IComponent {
    public:
        RectangleRenderComponent() : _color(colors::WHITE), _size{10.0f, 10.0f} {}
        RectangleRenderComponent(gfx::color_t color, float width, float height)
            : _color(color), _size{width, height} {}

        ~RectangleRenderComponent() = default;

        const gfx::color_t& getColor() const { return _color; }
        void setColor(const gfx::color_t& color) { _color = color; }

        float getWidth() const { return _size.first; }
        float getHeight() const { return _size.second; }
        void setSize(float width, float height) { _size = {width, height}; }

    private:
        gfx::color_t _color;
        std::pair<float, float> _size;
};

}  // namespace ecs

#endif /* !RECTANGLERENDERCOMPONENT_HPP_ */
