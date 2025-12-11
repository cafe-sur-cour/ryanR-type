/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Text
*/

#ifndef TEXT_HPP_
#define TEXT_HPP_

#include "base/UIElement.hpp"
#include "../../../common/types/Vector2f.hpp"
#include "../../../libs/Multimedia/IWindow.hpp"
#include "resourceManager/ResourceManager.hpp"
#include <memory>
#include <string>

namespace ui {

class Text : public UIElement {
public:
    Text(std::shared_ptr<ResourceManager> resourceManager);
    ~Text() override = default;

    void render() override;
    void update(float deltaTime) override;
    void setScale(UIScale scale) override;

    void setText(const std::string& text);
    std::string getText() const;

    void setTextColor(const gfx::color_t& color);
    void setFontSize(unsigned int size);
    void setFontPath(const std::string& path);

private:
    std::string _text;
    gfx::color_t _textColor;
    unsigned int _fontSize;
    unsigned int _baseFontSize;
    std::string _fontPath;
};

}  // namespace ui

#endif  // TEXT_HPP_
