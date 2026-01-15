/*
** EPITECH PROJECT, 2026
** R-Type
** File description:
** Image UI Element
*/

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include "base/UIElement.hpp"
#include <string>

namespace ui {

class Image : public UIElement {
public:
    Image(std::shared_ptr<ResourceManager> resourceManager);
    ~Image() override = default;

    void setTexturePath(const std::string& path);
    
    void render() override;
    void update(float deltaTime) override;

private:
    std::string _texturePath;
};

}  // namespace ui

#endif  // IMAGE_HPP_
