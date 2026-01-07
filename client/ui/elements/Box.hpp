/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Box
*/

#ifndef BOX_HPP_
#define BOX_HPP_

#include "base/UIElement.hpp"
#include "../../../common/interfaces/IWindow.hpp"

namespace ui {

class Box : public UIElement {
public:
    explicit Box(std::shared_ptr<ResourceManager> resourceManager);
    ~Box() override = default;

    void setBackgroundColor(const gfx::color_t& color);
    void setBorderColor(const gfx::color_t& color);
    void setBorderThickness(float thickness);
    void setCornerRadius(float radius);

    void render() override;

private:
    gfx::color_t _backgroundColor = gfx::color_t{40, 40, 60, 200};
    gfx::color_t _borderColor = gfx::color_t{80, 80, 120, 255};
    float _borderThickness = 2.0f;
    float _cornerRadius = 8.0f;
};

}  // namespace ui

#endif /* !BOX_HPP_ */
