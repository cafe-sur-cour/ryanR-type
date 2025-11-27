/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HitboxRenderComponent
*/

#ifndef HITBOXRENDERCOMPONENT_HPP_
#define HITBOXRENDERCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../../../libs/Multimedia/IWindow.hpp"

namespace ecs {

class HitboxRenderComponent : public AComponent {
    public:
        HitboxRenderComponent() : _color{255, 255, 255}, _outlineThickness(1.0f) {}
        HitboxRenderComponent(gfx::color_t color, float outlineThickness = 1.0f)
            : _color(color), _outlineThickness(outlineThickness) {}

        ~HitboxRenderComponent() = default;

        const gfx::color_t& getColor() const { return _color; }
        void setColor(const gfx::color_t& color) { _color = color; }

        float getOutlineThickness() const { return _outlineThickness; }
        void setOutlineThickness(float thickness) { _outlineThickness = thickness; }

    private:
        gfx::color_t _color;
        float _outlineThickness;
};

}  // namespace ecs

#endif /* !HITBOXRENDERCOMPONENT_HPP_ */
