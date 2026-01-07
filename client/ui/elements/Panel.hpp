/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Panel
*/

#ifndef PANEL_HPP_
#define PANEL_HPP_

#include "base/UIElement.hpp"
#include "../../../common/interfaces/IWindow.hpp"

namespace ui {

class Panel : public UIElement {
    public:
        Panel(std::shared_ptr<ResourceManager> resourceManager);
        virtual ~Panel() = default;

        void setScale(UIScale scale) override;

        void setBackgroundColor(gfx::color_t color);
        void setBorderColor(gfx::color_t color);
        void setBorderThickness(float thickness);

        void render() override;
        void update(float deltaTime) override;

    private:
        gfx::color_t _backgroundColor = {50, 50, 50, 255};
        gfx::color_t _borderColor = {100, 100, 100, 255};
        float _borderThickness = 2.0f;
};

}  // namespace ui

#endif /* !PANEL_HPP_ */
