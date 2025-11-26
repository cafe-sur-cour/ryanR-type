/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextureManager
*/

#include "TextureManager.hpp"
#include <iostream>

namespace gfx {

std::shared_ptr<sf::Texture> TextureManager::loadTexture(const std::string& path) {
    if (_failedTextures.find(path) != _failedTextures.end())
        return nullptr;
    auto it = _textureCache.find(path);
    if (it != _textureCache.end())
        return it->second;

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path)) {
        std::cerr << "[TextureManager] Failed to load texture: " << path << std::endl;
        _failedTextures.insert(path);
        return nullptr;
    }
    _textureCache[path] = texture;
    return texture;
}

void TextureManager::clearCache() {
    _textureCache.clear();
    _failedTextures.clear();
}

} // namespace gfx
