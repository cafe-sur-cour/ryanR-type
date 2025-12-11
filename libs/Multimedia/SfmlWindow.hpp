/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlWindow
*/

#ifndef SFMLWINDOW_HPP_
#define SFMLWINDOW_HPP_

#include <memory>
#include <SFML/Graphics.hpp>
#include "IWindow.hpp"
#include "TextureManager.hpp"
#include "FontManager.hpp"
#include "ShaderManager.hpp"
#include "RoundedRectangleShape.hpp"
#include "../../common/types/FRect.hpp"
#include "AssetManager/AssetManager.hpp"

class SfmlWindow : public gfx::IWindow {
    public:
        SfmlWindow(std::string title = "R-Type", size_t width = 800, size_t height = 600);
        ~SfmlWindow() override;
        void init() override;
        void display() override;
        void closeWindow() override;
        bool isOpen() override;
        void clear() override;
        void resizeWindow(size_t x, size_t y) override;

        void drawSprite(std::string asset, gfx::color_t color, std::pair<size_t, size_t> position) override;
        void drawText(std::string text, gfx::color_t color, std::pair<size_t, size_t> position, const std::string& fontPath, size_t fontSize, gfx::color_t outlineColor = {0, 0, 0}, float outlineThickness = 0.0f) override;
        std::pair<size_t, size_t> getTextSize(const std::string& text, const std::string& fontPath, size_t fontSize) override;
        void drawRectangleOutline(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;
        void drawFilledRectangle(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;
        void drawRoundedRectangleFilled(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, float radius) override;
        void drawRoundedRectangleOutline(gfx::color_t color, std::pair<size_t, size_t> position, std::pair<size_t, size_t> size, float radius) override;

        bool isMouseOver(std::pair<size_t, size_t> position, std::pair<size_t, size_t> size) override;
        std::pair<int, int> getWindowSize() override;

        void drawSprite(const std::string& texturePath, float x, float y, float scaleX = 1.0f, float scaleY = 1.0f) override;
        void drawSprite(const std::string& texturePath, float x, float y, const math::FRect frameRect, float scaleX = 1.0f, float scaleY = 1.0f) override;
        std::shared_ptr<sf::RenderWindow> getSfmlWindow();
        void updateView() override;
        void setViewCenter(float x, float y) override;
        math::Vector2f getViewCenter() override;
        math::Vector2f mapPixelToCoords(int x, int y) override;

        std::pair<int, int> getLogicalSize() const override;

        float getScaleFactor() const override;

        void addShaderFilter(const std::string& path) override;
        void removeShaderFilter(const std::string& path) override;
        void setShaderUniform(const std::string& filterPath, const std::string& name, float value) override;
        void setFramerateLimit(unsigned int fps) override;
        void setFullscreen(bool fullscreen) override;
        void setRenderQuality(float quality) override;
        void setCursor(bool isHand) override;

        void enableFilter(const std::string& filterName);
        void addFilter(const std::string& filterName);
        void removeFilter(const std::string& filterName);
        void disableAllFilters();
        bool isFilterActive(const std::string& filterName) const;
        ShaderManager& getShaderManager();

    private:
        std::shared_ptr<sf::RenderWindow> _window;
        std::shared_ptr<assets::AssetManager> _assetManager;
        gfx::TextureManager _textureManager;
        gfx::FontManager _fontManager;
        ShaderManager _shaderManager;
        sf::View _view;
        sf::RenderTexture _renderTexture;
        sf::Sprite _renderSprite;

        sf::Vector2f _viewportOffset;
        sf::Vector2f _viewportScale;
        float _renderQuality;
        unsigned int _renderWidth;
        unsigned int _renderHeight;
};

#endif /* !SFMLWINDOW_HPP_ */
