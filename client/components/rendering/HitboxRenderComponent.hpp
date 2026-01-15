/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** HitboxRenderComponent
*/

#ifndef HITBOXRENDERCOMPONENT_HPP_
#define HITBOXRENDERCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include "../../../common/interfaces/IWindow.hpp"
#include "../../colors.hpp"

namespace ecs {

class HitboxRenderComponent : public IComponent {
    public:
        HitboxRenderComponent() : _color{colors::WHITE}, _outlineThickness(1.0f) {}
        HitboxRenderComponent(
            gfx::color_t color, float outlineThickness = 1.0f
        ) : _color(color), _outlineThickness(outlineThickness) {}

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
