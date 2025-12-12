/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Slider
*/

#ifndef SLIDER_HPP_
#define SLIDER_HPP_

#include "../../core/AFocusableElement.hpp"
#include <string>
#include <functional>
#include "../../../../libs/Multimedia/IWindow.hpp"
#include "../../../constants.hpp"
#include "../../../colors.hpp"

namespace ui {

class Slider : public AFocusableElement {
    public:
        explicit Slider(std::shared_ptr<ResourceManager> resourceManager);
        virtual ~Slider() = default;

        void setMinValue(float minValue);
        void setMaxValue(float maxValue);
        void setValue(float value);
        float getValue() const;
        float getMinValue() const;
        float getMaxValue() const;

        void setStep(float step);
        float getStep() const;

        void setLabel(const std::string& label);
        const std::string& getLabel() const;
        void setLabelColor(const gfx::color_t& color);
        void setFontPath(const std::string& fontPath);
        void setBaseFontSize(size_t fontSize);
        size_t getBaseFontSize() const;
        void setShowPercentage(bool show);

        void setTrackColor(const gfx::color_t& color);
        void setFillColor(const gfx::color_t& color);
        void setHandleColor(const gfx::color_t& color);
        void setHandleHoveredColor(const gfx::color_t& color);
        void setHandleFocusedColor(const gfx::color_t& color);

        void setOnValueChanged(std::function<void(float)> callback);

        virtual void render() override;
        virtual void handleInput(const math::Vector2f& mousePos, bool mousePressed) override;
        virtual void onActivated() override;
        virtual bool onNavigateLeft() override;
        virtual bool onNavigateRight() override;

        void incrementValue();
        void decrementValue();

    private:
        float _minValue = 0.0f;
        float _maxValue = 1.0f;
        float _value = 0.5f;
        float _step = 0.1f;
        float _visualNormalizedValue = 0.5f;

        std::string _label;
        gfx::color_t _labelColor = colors::SLIDER_LABEL;
        std::string _fontPath = "assets/fonts/arial.ttf";
        size_t _baseFontSize = constants::BUTTON_FONT_SIZE_BASE;
        float _outlineThickness = 2.0f;
        bool _showPercentage = true;

        gfx::color_t _trackColor = colors::SLIDER_TRACK;
        gfx::color_t _fillColor = colors::SLIDER_FILL;
        gfx::color_t _handleColor = colors::SLIDER_HANDLE;
        gfx::color_t _handleHoveredColor = colors::SLIDER_HANDLE_HOVER;
        gfx::color_t _handleFocusedColor = colors::SLIDER_HANDLE_FOCUSED;

        std::function<void(float)> _onValueChanged;

        bool _isDragging = false;
        bool _wasMousePressed = false;

        float getNormalizedValue() const;
        void setNormalizedValue(float normalized);
        gfx::color_t getCurrentHandleColor() const;
        size_t getFontSize() const;
        float getHandleRadius() const;
        float getTrackHeight() const;
        float getLabelHeight() const;
};

}  // namespace ui

#endif /* !SLIDER_HPP_ */
