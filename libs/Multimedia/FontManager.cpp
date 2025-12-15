/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** FontManager
*/

#include "FontManager.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

namespace gfx {

FontManager::FontManager(std::shared_ptr<assets::AssetManager> assetManager)
    : _assetManager(assetManager) {
    this->_fontCache = std::unordered_map<std::string, std::shared_ptr<sf::Font>>();
    this->_fontDataCache = std::unordered_map<std::string, std::vector<unsigned char>>();
}

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
    _fontDataCache[path] = fontData;

    auto font = std::make_shared<sf::Font>();
    if (!_fontDataCache[path].empty() &&
        !font->openFromMemory(_fontDataCache[path].data(), _fontDataCache[path].size())) {
        std::cerr << "[FontManager] Failed to load font from memory: "
                   << path << std::endl;
        return nullptr;
    }
    _fontCache[path] = font;
    return font;
}

}  // namespace gfx
