/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** FontManager
*/

#include "FontManager.hpp"
#include <iostream>
#include <memory>
#include <string>

namespace gfx {

FontManager::FontManager(std::shared_ptr<assets::AssetManager> assetManager)
    : _assetManager(assetManager) {}

std::shared_ptr<sf::Font> FontManager::getFont(const std::string& path) {
    auto it = _fontCache.find(path);
    if (it != _fontCache.end())
        return it->second;

    std::string normalizedPath = path;
    if (normalizedPath.find("./assets/") == 0)
        normalizedPath = normalizedPath.substr(9);
    if (normalizedPath.find("assets/") == 0)
        normalizedPath = normalizedPath.substr(7);

    auto asset = _assetManager->getAsset(normalizedPath);
    if (!asset) {
        std::cerr << "[FontManager] Failed to load asset: "
            << normalizedPath << std::endl;
        return nullptr;
    }

    std::vector<unsigned char> fontData = asset->data;

    auto font = std::make_shared<sf::Font>();
    if (!font->openFromMemory(fontData.data(), fontData.size())) {
        std::cerr << "[FontManager] Failed to load font from memory: "
                   << path << std::endl;
        return nullptr;
    }
    _fontCache[path] = font;
    return font;
}

}  // namespace gfx
