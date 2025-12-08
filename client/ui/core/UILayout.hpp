/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** UILayout
*/

#ifndef UILAYOUT_HPP_
#define UILAYOUT_HPP_

#include <memory>
#include <vector>
#include "../elements/base/UIElement.hpp"
#include "../../../common/types/Vector2f.hpp"

namespace ui {

enum class LayoutDirection {
    Horizontal,
    Vertical
};

enum class LayoutAlignment {
    Start,
    Center,
    End
};

enum class AnchorX {
    None,
    Left,
    Center,
    Right
};

enum class AnchorY {
    None,
    Top,
    Center,
    Bottom
};

struct LayoutConfig {
    LayoutDirection direction = LayoutDirection::Vertical;
    LayoutAlignment alignment = LayoutAlignment::Start;
    float spacing = 0.0f;
    math::Vector2f padding = math::Vector2f(0.0f, 0.0f);
    math::Vector2f offset = math::Vector2f(0.0f, 0.0f);
    bool autoResize = false;
    AnchorX anchorX = AnchorX::None;
    AnchorY anchorY = AnchorY::None;
};

class UILayout : public UIElement {
    public:
        UILayout(std::shared_ptr<ResourceManager> resourceManager, const LayoutConfig& config = LayoutConfig());
        ~UILayout() override = default;

        void addElement(std::shared_ptr<UIElement> element);
        void removeElement(std::shared_ptr<UIElement> element);
        void clearElements();

        void setDirection(LayoutDirection direction);
        void setAlignment(LayoutAlignment alignment);
        void setSpacing(float spacing);
        void setPadding(const math::Vector2f& padding);
        void setOffset(const math::Vector2f& offset);
        void setAutoResize(bool autoResize);
        void setAnchor(AnchorX anchorX, AnchorY anchorY);

        LayoutDirection getDirection() const;
        LayoutAlignment getAlignment() const;
        float getSpacing() const;
        math::Vector2f getPadding() const;
        bool isAutoResize() const;

        void updateLayout();

        void setScale(UIScale scale) override;

        void render() override;
        void update(float deltaTime) override;

        float getScaledSpacing() const;
        void applyAnchor();    private:
        LayoutConfig _config;
        std::vector<std::shared_ptr<UIElement>> _layoutElements;

        void calculatePositions();
        float getTotalSize() const;
        math::Vector2f calculateElementPosition(size_t index, float totalSize) const;
};

}  // namespace ui

#endif /* !UILAYOUT_HPP_ */
